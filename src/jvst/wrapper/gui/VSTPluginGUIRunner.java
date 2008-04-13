package jvst.wrapper.gui;

import java.lang.reflect.Constructor;

import javax.swing.SwingUtilities;
import jvst.wrapper.VSTPluginAdapter;
import jvst.wrapper.VSTPluginGUIAdapter;


public class VSTPluginGUIRunner implements VSTPluginGUI {

	public boolean WindowAttached;
	public boolean libraryOk = false;	
	
	protected VSTPluginGUIAdapter gui = null;

	
	//GENERAL: only use invokeLater, invokeAndWait tends to cause deadlocks, if invoked from the main thread the jvms runloop is blocked 
	// and a deadlock is produced (invokeAndWait waits, but nothing is done on the awt event thread since the jvm waits as well)
	public void loadVSTGUI(String clazzname, VSTPluginAdapter p) {
		//use the VSTiClassLoader
		final ClassLoader cl = this.getClass().getClassLoader();
		final String classname = clazzname.replace('/', '.');
		final VSTPluginGUIRunner thi = this;
		final VSTPluginAdapter plug = p;
		
		log("Initializing CLASSLOAD Plugin GUI=" + classname);
		try {
			//SwingUtilities.invokeLater(new Runnable() {
			SwingUtilities.invokeAndWait(new Runnable() {
			    public void run() {
			        try {
						Class clazz = cl.loadClass(classname);
						Constructor c = clazz.getConstructor(new Class[]{VSTPluginGUIRunner.class, VSTPluginAdapter.class});
						log("BEFORE Initializing Plugin GUI on AWT-Event Thread CLAZZ=" + clazz + " constructor=" + c);
			        	//gui = (VSTPluginGUIAdapter)clazz.newInstance();
						gui = (VSTPluginGUIAdapter)c.newInstance(new Object[]{thi,plug});
			        	log("Plugin GUI was initialized on Event Thread! GUI=" + gui);
					} 
			        catch (Exception e) {
						e.printStackTrace();
					} 
			    }
			});
		} 
		catch (Throwable t) {
			t.printStackTrace();
		}	
		
		//since all locks caused deadlocks... busy wait here
		//-- delay thread so that succeeding init() does have a chance, otherwise init() is likely to fail
		//TODO: maybe use invokeAndWait again here? seemed to work before
		
		/*
		while(gui==null) {
			try {
				log("waiting another round");
				Thread.sleep(300);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		*/
	}
	
	
	
	//need to block this call until init is done
	//BUT NOT ON THIS THREAD (this thread seems to run the jvm event loop, if it is suspended 
	//everything deadlocks). same for the rest of methods, all of them are called from c on the main thread
	public void open() {	
		if (this.gui==null) {
			log("** ERROR-open: (VSTPluginGUIRunner) gui is NULL!");
			return;
		}
		
		log("about to OPEN GUI on AWT event thread");
		try {
			SwingUtilities.invokeLater(new Runnable() {
	            public void run() {
	                gui.open();
	            }
	        });	
		} 
		catch (Throwable t) {
			t.printStackTrace();
		}

		log("GUI OPEN OKAY");
	}
	
	public void close() {
		if (this.gui==null) {
			log("** ERROR-close: (VSTPluginGUIRunner) gui is NULL!");
			return;
		}
		
		log("about to CLOSE GUI on AWT event thread");
		try {
			SwingUtilities.invokeLater(new Runnable() {
	            public void run() {
	                gui.close();
	            }
	        });	
		} 
		catch (Throwable t) {
			t.printStackTrace();
		}	
	}
	
	public void destroy() {
		if (this.gui==null) {
			log("** ERROR-destroy: (VSTPluginGUIRunner) gui is NULL!");
			return;
		}
		
		log("about to DESTROY GUI on AWT event thread");
		try {
			SwingUtilities.invokeLater(new Runnable() {
	            public void run() {
	                gui.destroy();
	            }
	        });	
		} 
		catch (Throwable t) {
			t.printStackTrace();
		}
	}

/*
	public void undecorate() {
		if(initDone==false) {
			log("delaying run of gui.undecorate()");
			doUndecorate=true;
			return;
		}
		if (this.gui==null) {
			log("** ERROR-undeco: (VSTPluginGUIRunner) gui is NULL!");
			return;
		}
		
		log("about to UNDECORATE GUI on AWT event thread");
		try {
			SwingUtilities.invokeLater(new Runnable() {
	            public void run() {
	                gui.undecorate();
	            }
	        });	
		} 
		catch (Throwable t) {
			t.printStackTrace();
		}	
		
		this.doUndecorate=false;
		log("GUI UNDECORATE OKAY");
	}
	
	
	public int getWidth() {
		final int[] ret = new int[]{100};
		
		if (this.gui==null) {
			log("** ERROR-getw: getWidth (VSTPluginGUIRunner) gui is NULL!");
			return ret[0];
		}
		
		log("about to getWidth GUI on AWT event thread");
		try {
			SwingUtilities.invokeLater(new Runnable() {
	            public void run() {
	            	//use reference
	                ret[0] = gui.getWidth();
	            }
	        });	
		} 
		catch (Throwable t) {
			t.printStackTrace();
		}	
		
		log("GUI getWidth = " + ret[0] + " OKAY");
		return ret[0];
	}
	
	public int getHeight() {
		final int[] ret = new int[]{100};
		
		if (this.gui==null) {
			log("** ERROR-geth: getHeight (VSTPluginGUIRunner) gui is NULL!");
			return ret[0];
		}
		
		log("about to getHeight GUI on AWT event thread");
		try {
			SwingUtilities.invokeLater(new Runnable() {
	            public void run() {
	            	//use reference
	                ret[0] = gui.getHeight();
	            }
	        });	
		} 
		catch (Throwable t) {
			t.printStackTrace();
		}	
		
		log("GUI getHeight = " + ret[0] + " OKAY");
		return ret[0];
	}
*/
	
	// ***********************************************************************
	protected void log(String s) {
		VSTPluginAdapter.log(s);
	}
	
}
