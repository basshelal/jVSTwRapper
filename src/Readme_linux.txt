Writing VST plugins in Java on LINUX mini HOW-TO
--------------------------------------------------

1. Make sure you have a JAVA_HOME environment variable that is set to the correct value
   e.g. in your .bashrc add: export JAVA_HOME=/usr/lib/jvm/java-6-sun/jre/
   If you have a jdk installed (like me), JAVA_HOME should point to the location of your JRE. This is 
   usually done by appending /jre to the path of your jdk, like in the example above.
   
2. On a fresh installed sun jvm, you MAY need to tweak your LD_LIBRARY_PATH to contain 
	the path to $PATH_TO_JAVA/jre/lib/i386/
   
   Do this when you see the following error in jvstwrapper_log.txt:
   ** java.lang.UnsatisfiedLinkError: Can't load library: ### some .so file ###
   
3. Write your plugin using the wrapper api.

4. Specify your plugin main class in the jvstwrapper.ini file 

5. Make sure that the .ini has the same 
   name as the .so file and is stored in the same directory.
   
6. Look for problems. If you left IsLoggingEnabled=1 in the ini file, the log files will 
	appear in the same directory as the plugin .so file is.


NOTE: On Ubuntu Edgy Eft (6.10), I had problems with java GUIs. They completely froze the host applications. If 
	  you encounter the same problem, consider upgrading your Linux distribution (or disable the java GUI for the 
	  plugins by commenting out PluginUIClass in the .ini file).
	  Ubuntu Feisty Fawn (7.04) works perfect!



*** PLEASE USE THE FORUM FOR QUESTIONS ***
http://sourceforge.net/forum/forum.php?forum_id=318265