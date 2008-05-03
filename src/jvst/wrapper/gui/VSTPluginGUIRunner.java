package jvst.wrapper.gui;

import java.lang.reflect.Constructor;

import javax.swing.SwingUtilities;
import jvst.wrapper.VSTPluginAdapter;
import jvst.wrapper.VSTPluginGUIAdapter;


public class VSTPluginGUIRunner implements VSTPluginGUI {

	public boolean WindowAttached;
	public boolean libraryOk = false;	
	
	protected VSTPluginGUIAdapter gui = null;

	// yeah, i know: type save enums would be better here. but those arent java 1.3 compatible 
	// and im too lazy to create a separate class...
	private int OPEN = 0;
	private int CLOSE = 1;
	private int DESTROY = 2;
	private int UNDECORATE = 3;
	private int GETWIDTH = 4;
	private int GETHEIGHT = 5;

	
	
	//GENERAL: only use invokeLater, invokeAndWait tends to cause deadlocks, if invoked from the main thread the jvms runloop is blocked 
	// and a deadlock is produced (invokeAndWait waits, but nothing is done on the awt event thread since the jvm waits as well)
	
	//we throw an exception here so that any errors can be detected on the native side. 
	//if an error occurs, the wrapper disables the java gui and uses the VST default gui, rendered by the host app
	public void loadVSTGUI(String clazzname, VSTPluginAdapter p) throws Exception {
		//use the VSTiClassLoader
		final ClassLoader cl = this.getClass().getClassLoader();
		final String classname = clazzname.replace('/', '.');
		final VSTPluginGUIRunner thi = this;
		final VSTPluginAdapter plug = p;
		
		log("Initializing CLASSLOAD Plugin GUI=" + classname);

		//SwingUtilities.invokeLater(new Runnable() {
		//tests showed that invokeandwait is okay (running on a newly created thread on mac --> okay here)
		SwingUtilities.invokeAndWait(new Runnable() {
		    public void run() {
		        try {
					Class clazz = cl.loadClass(classname);
					Constructor c = clazz.getConstructor(new Class[]{VSTPluginGUIRunner.class, VSTPluginAdapter.class});
					log("BEFORE Initializing Plugin GUI on AWT-Event Thread CLAZZ=" + clazz + " constructor=" + c);
					gui = (VSTPluginGUIAdapter)c.newInstance(new Object[]{thi,plug});
		        	log("Plugin GUI was initialized on Event Thread! GUI=" + gui);
				} 
		        catch (Exception e) {
					e.printStackTrace();
				} 
		    }
		});
	}
	
	
	
	//need to block this call until init is done
	//BUT NOT ON THIS THREAD (this thread seems to run the jvm event loop, if it is suspended 
	//everything deadlocks). 
	//same for the rest of methods, all of them are called from c on the main thread --> never use invokeandwait() here !!!
	//at least on the mac, win and linux are fine with invokeandwait...
	public void open() {	
		runLater(OPEN);
	}
	
	public void close() {
		runLater(CLOSE);
	}
	
	public void destroy() {
		runLater(DESTROY);
	}

	
	
	// starting from here, these methods are called with invokeAndWait() --> dont ever call them on mac! will cause deadlock
	// but since we dont do window embedding on mac, we dont use these methods there. they are save on win and linux. 
	public void undecorate() {
		runAndWait(UNDECORATE);
	}

	public int getWidth() {
		return runAndWait(GETWIDTH);
	}
	
	public int getHeight() {
		return runAndWait(GETHEIGHT);
	}

	
	
	
	
	private void runLater(final int method) {
		if (this.gui==null) {
			log("** ERROR method=" + method + " (VSTPluginGUIRunner.runLater) gui is NULL!");
			return;
		}
		
		log("about to run method=" + method + " on AWT event thread");
		try {
			SwingUtilities.invokeLater(new Runnable() {
	            public void run() {
	            	switch (method) {
						case 0:
							gui.open();
							break;
						case 1:
							gui.close();
							break;
						case 2:
							gui.destroy();
							break;
						default:
							log("** ERROR: unknown method: " + method);
							break;
					}
	            }
	        });	
		} 
		catch (Throwable t) {
			t.printStackTrace();
		}
	}
	
	private int runAndWait(final int method) {
		final int[] ret = new int[]{100};
		
		if (this.gui==null) {
			log("** ERROR method=" + method + " (VSTPluginGUIRunner.runAndWait) gui is NULL!");
			return ret[0];
		}
		
		log("about to run method=" + method + " on AWT event thread");
		try {
			SwingUtilities.invokeAndWait(new Runnable() {
	            public void run() {
	            	switch (method) {
						case 3:
			                gui.undecorate();
							break;
						case 4:
			                ret[0] = gui.getWidth();
							break;
						case 5:
			                ret[0] = gui.getHeight();
							break;
						default:
							log("** ERROR: unknown method: " + method);
							break;
	            	}
	            }
	        });	
		} 
		catch (Throwable t) {
			t.printStackTrace();
		}	
		
		log("runAndWait=" + ret[0] + " OKAY");
		return ret[0];
	}


	
	// ***********************************************************************
	protected void log(String s) {
		VSTPluginAdapter.log(s);
	}
	
}
