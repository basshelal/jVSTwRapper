Writing VST plugins in Java on the Mac mini HOW-TO
--------------------------------------------------

1. Write your plugin using the wrapper api.

2. Compile your plugin and include it in the Resources folder of the jvstwrapper.vst plugin bundle. 
The Resources Directory will be added to the classpath by default. You can add additional classpath in the jVSTwRapper.ini file 
(WARNING: you could leave this blank, but DON'T do it for now, put anything).

3. Specify your plugin main class in the jvstwrapper.jnilib.ini file 

4. Rename the bundle to YourPlugsName.vst. ALso, rename libjvstwrapper.jnilib to YourPlugsName.jnilib. 
   But make sure to keep the .jnilib extension for the wrapper library. ALSO, edit the Info.plist file 
   in the bundle to reflect the new CFBundleExecutable (e.g. YourPlugsName.jnilib) and CFBundleName values.

5. Look for problems. If you left IsLoggingEnabled=1 in the ini file, the log files will appear in the Resources directory and the MacOS directory inside the .vst bundle


NOTE:
------
This release (0.9a) favoured stability over functionality. This is the reason why 
custom Java guis are currently disabled on the Mac. However, the Mac and windows 
releases are otherwise in sync, use exactly the same codebase and are both very stable. 
As soon as we get the AWT integration on the mac back to work stable, we will add it to the mac release. 



*** PLEASE USE THE FORUM FOR QUESTIONS ***
http://sourceforge.net/forum/forum.php?forum_id=318265