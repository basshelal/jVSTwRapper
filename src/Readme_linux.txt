Writing VST plugins in Java on LINUX mini HOW-TO
--------------------------------------------------

1. Make sure you have a JAVA_HOME environment variable that is set to the correct value
   e.g. in your .bashrc add: export JAVA_HOME=/usr/lib/jvm/java-6-sun/jre/
   
2. On a fresh installed sun jvm, you may need to create a symbolic link to libmawt.so as follows:
   # cd /usr/lib/jvm/java-6-sun/jre/lib/i386/xawt
   # sudo ln -s ./libmawt.so ../libmawt.so
   
   it may even be necessary to create a link to /usr/lib
   # sudo ln -s ./libmawt.so /usr/lib/libmawt.so
   
   in the worst case, you need to add a link to /xawt in the 
   folder where your java plugins are 
   
   As an alternative, you can set your LD_LIBRARY_PATH to contain the path to /xawt/libmawt.so
   
   Do this when you see the following error in jvstwrapper_log.txt:
   ** java.lang.UnsatisfiedLinkError: Can't load library: $[SOME PATH]/xawt/libmawt.so
   
3. Write your plugin using the wrapper api.

4. On Linux, a special name convention for the .so file is necessary: 
   The name pattern is: jvst*.so 
   You can rename the .so file to whatever ou want, just make sure the 
   first 4 letters are 'jvst', all lowercase. Also, make sure that the .ini has the same 
   name as the .so file and is stored in the same directory.

4. Specify your plugin main class in the jvstwrapper.ini file 

5. Look for problems. If you left IsLoggingEnabled=1 in the ini file, the log files will appear in the 
   Resources directory and the MacOS directory inside the .vst bundle



*** PLEASE USE THE FORUM FOR QUESTIONS ***
http://sourceforge.net/forum/forum.php?forum_id=318265