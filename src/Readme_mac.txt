Writing VST plugins in Java on the Mac mini HOW-TO
--------------------------------------------------

1. Write your plugin using the wrapper api.

2. Compile your plugin and include it in the Resources folder of the jVSTwRapper.vst plugin bundle. 
The Resources Directory will be added to the classpath by default. You can add additional classpath in the jVSTwRapper.ini file 
(WARNING: you could leave this blank, but DON'T do it for now, put anything).

3. Specify your plugin main class and (optionally) main GUI class in the jVSTwRapper.ini file (Remove the entry if your plug has no GUI)

4. Rename the  plugin

5. Look for problems. If you left IsLoggingEnabled=true in the ini file, the log files will appear in the Resources directory inside the .vst bundle