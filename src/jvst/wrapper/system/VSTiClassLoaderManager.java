/*
 * Created on Jul 27, 2004 by dmartin
 * for project: EclipseVSTwRapper 
 */
package jvst.wrapper.system;

//import java.io.BufferedOutputStream;
import java.io.File;
//import java.io.FileOutputStream;
//import java.io.PrintStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.StringTokenizer;

/**
 * @author dmartin
 */
public class VSTiClassLoaderManager {

	private static Map classLoaders = new HashMap(10);
	
	/*
	private static PrintStream str;
	static {
		try {
			str = new PrintStream(new BufferedOutputStream(
					new FileOutputStream("VSTiLoader.txt", false)));
			System.setErr(str);
			System.setOut(str);
		} catch (Throwable t) {
			//ignore
		}
	}

	public static void log(String m) {
		str.println(m);
		str.flush();
	}
	*/
	
	
	public static Object createVSTiInstance(Class clazz)
			throws InstantiationException, IllegalAccessException {
		Object instance = clazz.newInstance();
		return instance;
	}

	public static Class loadVSTiClass(String nativeLib, String fqClassName,
			String classPath) throws ClassNotFoundException {
		if (fqClassName == null)
			throw new IllegalArgumentException("fqClassName is NULL!");
		if (nativeLib == null)
			throw new IllegalArgumentException("nativeLib is NULL!");
		if (classPath == null)
			throw new IllegalArgumentException("classPath is NULL!");

		
		ArrayList urlList = new ArrayList(10);
		StringTokenizer strTok = new StringTokenizer(classPath, System.getProperty("path.separator", ";"), false);
		while (strTok.hasMoreTokens()) {
			String tok = strTok.nextToken();
			File f = new File(tok);
			if (!f.exists())
				continue;

			try {
				//log("Adding URL:" + f.toURL().toString());
				urlList.add(f.toURL());
			} catch (MalformedURLException e) {
				throw new IllegalArgumentException(e.toString());
			}
		}

		URL[] urls = (URL[])urlList.toArray(new URL[1]);
		
		fqClassName = fqClassName.replaceAll("/", ".");

		ClassLoader loader = (ClassLoader) classLoaders.get(nativeLib);

		if (loader == null) {
			//log("new Loader for: " + nativeLib);

			//no associated cl so far. plug is being loaded for the first time!
			loader = new VSTiClassLoader(urls);
			Class clazz = loader.loadClass(fqClassName);

			classLoaders.put(nativeLib, loader);

			//log("Class Loader=" + clazz.getClassLoader());

			return clazz;
		} else {
			//log("Reusing Loader for: " + nativeLib);

			//this plug has been loaded.. reuse class loader so that the native
			//calls are being redirected to the correct library!
			Class clazz = loader.loadClass(fqClassName);

			//log("Class Loader=" + clazz.getClassLoader());

			return clazz;
		}
	}

}