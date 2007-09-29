Writing VST plugins in Java on the Mac mini HOW-TO
--------------------------------------------------

1. Write your plugin using the wrapper api.

2. Compile your plugin and include it in the Resources folder of the jvstwrapper.vst plugin bundle. 
   The Resources directory will be added to the classpath by default. You can add additional classpath 
   in the jVSTwRapper.ini file 
   (WARNING: you could leave this blank, but DON'T do it for now, put anything).

3. Specify your plugin main class in the jvstwrapper.jnilib.ini file 

4. Rename the bundle to YourPlugsName.vst. ALso, rename libjvstwrapper.jnilib to YourPlugsName.jnilib. 
   But make sure to keep the .jnilib extension for the wrapper library. ALSO, edit the Info.plist file 
   in the bundle to reflect the new 'CFBundleExecutable' (e.g. YourPlugsName.jnilib) and 'CFBundleName' values.

5. Look for problems. If you left IsLoggingEnabled=1 in the ini file, the log files will appear in the 
   Resources directory and the MacOS directory inside the .vst bundle


NOTE:
------
In this release, VST GUI (Swing) support is back, but NOT ENABLED in the .ini file since there 
MAY be still a few hosts that freeze/crash when loading the Java GUI. 

*** Note that Java GUIs require at least Java 1.5.0_07 (Mac OSX 10.4.8 or higher) to be installed on your system ***
Use 'java -version' in a terminal window to check your version, and 'Software Update' to install 
if you have a lower version.

To enable the GUI for e.g. the JayDLay demo plugin, simply uncomment (remove the '#') from the line 
#PluginUIClass=jvst/examples/jaydlay/JayDLayGUI
in the .ini file.


Please report to the forum any host that causes problems with Java GUIs. I will include them
in my tests. 


*** PLEASE USE THE FORUM FOR QUESTIONS ***
http://sourceforge.net/forum/forum.php?forum_id=318265
