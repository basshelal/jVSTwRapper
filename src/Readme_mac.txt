Writing VST Plugins in Java on the Mac -- Mini How-To
--------------------------------------------------

1. Write your plugin using the wrapper api.

2. Compile your plugin and include it in the Resources folder of the jvstwrapper.vst plugin bundle. 
   The Resources directory will be added to the classpath by default. You can add additional classpath entries 
   in the jVSTwRapper.ini file 
   (WARNING: you could leave this blank, but DON'T do it for now, put anything).

3. Specify your plugin main class in the jvstwrapper.jnilib.ini file 

4. Rename the bundle to YourPlugsName.vst. Also, rename libjvstwrapper.jnilib to YourPlugsName.jnilib. 
   But make sure to keep the .jnilib extension for the wrapper library. ALSO, edit the Info.plist file 
   in the bundle to reflect the new 'CFBundleExecutable' (e.g. YourPlugsName.jnilib) and 'CFBundleName' values.

5. Look for problems. If you left IsLoggingEnabled=1 in the ini file, the log files will appear in the 
   Resources directory and the MacOS directory inside the .vst bundle



Wrapping you jVSTwRapper VST plugin as Audio Unit (AU)
--------------------------------------------------------------

1. Install (i.e. copy the bundle) your jVSTwRapper VST plugin 
   to /Library/Audio/Plug-Ins/VST/ or ~/Library/Audio/Plug-Ins/VST/

2. Start the "VSTAU Manager" application, find you plugin in the list and click "install". 
   This will create a wrapper AU plugin for the selected VST plug. 

3. You can find the created AU plugin -- named "OriginalVSTPluginName (VSTAU)" -- 
   in ~/Library/Audio/Plug-Ins/Components/ 

4. To distribute your plugin as AU, you *** need to ship the VST version as well *** 
   -- the AU version is only a wrapper that loads the VST version when started. 
   So, to distribute your plugin add both, the VST and the AU version 
   to a single archive. To install, simply copy both bundles to the folders /VST or /Components 
   in either /Library/Audio/Plug-Ins/ or ~/Library/Audio/Plug-Ins/

--> You may also try this process with the jVST sample plugins that ship with the 
    jVSTwRapper download to see how it works



NOTE:
------
*** Java GUIs require at least Java 1.5.0_07 (Mac OSX 10.4.8 or higher) to be installed on your system ***
Use 'java -version' in a terminal window to check your version, and 'Software Update' to install 
if you have a lower version.

Please report to the forum any host application that causes problems with Java GUIs. I will include them
in my tests. 


*** PLEASE USE THE FORUM FOR QUESTIONS ***
http://sourceforge.net/forum/forum.php?forum_id=318265
