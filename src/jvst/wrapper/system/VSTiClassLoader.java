/*
 * Created on Jul 27, 2004 by dmartin
 * for project: EclipseVSTwRapper 
 */
package jvst.wrapper.system;

import java.net.URL;
import java.net.URLClassLoader;

/**
 * @author dmartin
 */
public class VSTiClassLoader extends URLClassLoader {

	public VSTiClassLoader(URL[] urls) {
		super(urls);
	}
	
	/* (non-Javadoc)
	 * @see java.lang.ClassLoader#findClass(java.lang.String)
	 */
	protected Class findClass(String arg0) throws ClassNotFoundException {
		//VSTiClassLoaderManager.log("findClass: " + arg0); 
		
		// TODO Auto-generated method stub
		return super.findClass(arg0);
	}
	/* (non-Javadoc)
	 * @see java.lang.ClassLoader#findLibrary(java.lang.String)
	 */
	protected String findLibrary(String arg0) {
		//VSTiClassLoaderManager.log("findLibrary: " + arg0);
		
		// TODO Auto-generated method stub
		return super.findLibrary(arg0);
	}
	/* (non-Javadoc)
	 * @see java.lang.ClassLoader#loadClass(java.lang.String, boolean)
	 */
	protected synchronized Class loadClass(String arg0, boolean arg1)
			throws ClassNotFoundException {
		//VSTiClassLoaderManager.log("loadClass: " + arg0 + " bool=" + arg1);
		
		// TODO Auto-generated method stub
		return super.loadClass(arg0, arg1);
	}
	/* (non-Javadoc)
	 * @see java.lang.ClassLoader#loadClass(java.lang.String)
	 */
	public Class loadClass(String arg0) throws ClassNotFoundException {
		//VSTiClassLoaderManager.log("loadClass: " + arg0);
		
		// TODO Auto-generated method stub
		return super.loadClass(arg0);
	}
}
