/*
 * Created on Jul 27, 2004
 */
package jvst.wrapper.system;

import java.io.PrintWriter;
import java.io.StringWriter;

/**
 * @author dmartin
 *  
 */
public class ExceptionUtililities {
	
	//util to report a stacktrace... is called from the native lib.
	public static String getStackTrace(Throwable t) {
		StringWriter strw = new StringWriter();
		t.printStackTrace(new PrintWriter(strw));
		return strw.getBuffer().toString();
	}
	
}