# This is the LINUX makefile
# Other Platforms use IDE build-in features for compilation (Mac: XCode, Win: MS Visual Studio)
JAVA_HOME=/usr/lib/jvm/java-6-sun


all:	release

debug:
#	debug build option as executable so that all errors are reported!
#	g++ -o ./bin/jvstwrapper.so -g -Wall -ldl -fvisibility=hidden -fvisibility-inlines-hidden \
#	-I/usr/include -I./src/src/public.sdk/vst2.x -I./src/src/ladspa -I$(JAVA_HOME)/include \
#	-I$(JAVA_HOME)/include/linux -I./src/src/pluginterfaces/vst2.x -I./src/src \
#	src/src/jVSTwRapperMAIN.cpp src/src/ConfigFileReader.cpp src/src/JNIUtils.cpp \
#	src/src/VSTV10ToHost.cpp src/src/VSTV10ToPlug.cpp src/src/VSTV20ToHost.cpp \
#	src/src/VSTV20ToPlug.cpp src/src/VSTV21ToHost.cpp src/src/VSTV21ToPlug.cpp \
#	src/src/VSTV22ToHost.cpp src/src/VSTV22ToPlug.cpp src/src/VSTV23ToHost.cpp \
#	src/src/VSTV23ToPlug.cpp src/src/VSTV24ToHost.cpp src/src/VSTV24ToPlug.cpp \
#	src/src/VSTGUIWrapper.cpp src/src/public.sdk/vst2.x/audioeffect.cpp \
#	src/src/public.sdk/vst2.x/audioeffectx.cpp src/src/vstgui/aeffguieditor.cpp \
#	src/src/vstgui/vstgui.cpp src/src/ladspa/jLADSPAwRapperMAIN.cpp

#	the real thing is here...
	g++ -o ./bin/jvstwrapper.so -shared -fPIC -g -Wall -ldl -fvisibility=hidden -fvisibility-inlines-hidden \
	-I/usr/include -I./src/src/public.sdk/vst2.x -I./src/src/ladspa -I$(JAVA_HOME)/include \
	-I$(JAVA_HOME)/include/linux -I./src/src/pluginterfaces/vst2.x -I./src/src \
	src/src/jVSTwRapperMAIN.cpp src/src/ConfigFileReader.cpp src/src/JNIUtils.cpp \
	src/src/VSTV10ToHost.cpp src/src/VSTV10ToPlug.cpp src/src/VSTV20ToHost.cpp \
	src/src/VSTV20ToPlug.cpp src/src/VSTV21ToHost.cpp src/src/VSTV21ToPlug.cpp \
	src/src/VSTV22ToHost.cpp src/src/VSTV22ToPlug.cpp src/src/VSTV23ToHost.cpp \
	src/src/VSTV23ToPlug.cpp src/src/VSTV24ToHost.cpp src/src/VSTV24ToPlug.cpp \
	src/src/VSTGUIWrapper.cpp src/src/public.sdk/vst2.x/audioeffect.cpp \
	src/src/public.sdk/vst2.x/audioeffectx.cpp src/src/vstgui/aeffguieditor.cpp \
	src/src/vstgui/vstgui.cpp src/src/ladspa/jLADSPAwRapperMAIN.cpp
	
# stuff to try
# add -rdynamic again???
# -fpic instead of -fPIC

# dynamic loader debug:
# export LD_DEBUG=bindings
# export LD_DEBUG_OUTPUT=loader_debug.txt

release:
	g++ -o ./bin/jvstwrapper.so -shared -fPIC -O2 -Wall -ldl -fvisibility=hidden -fvisibility-inlines-hidden \
	-I/usr/include -I./src/src/public.sdk/vst2.x -I./src/src/ladspa -I$(JAVA_HOME)/include \
	-I$(JAVA_HOME)/include/linux -I./src/src/pluginterfaces/vst2.x -I./src/src \
	src/src/jVSTwRapperMAIN.cpp src/src/ConfigFileReader.cpp src/src/JNIUtils.cpp \
	src/src/VSTV10ToHost.cpp src/src/VSTV10ToPlug.cpp src/src/VSTV20ToHost.cpp \
	src/src/VSTV20ToPlug.cpp src/src/VSTV21ToHost.cpp src/src/VSTV21ToPlug.cpp \
	src/src/VSTV22ToHost.cpp src/src/VSTV22ToPlug.cpp src/src/VSTV23ToHost.cpp \
	src/src/VSTV23ToPlug.cpp src/src/VSTV24ToHost.cpp src/src/VSTV24ToPlug.cpp \
	src/src/VSTGUIWrapper.cpp src/src/public.sdk/vst2.x/audioeffect.cpp \
	src/src/public.sdk/vst2.x/audioeffectx.cpp src/src/vstgui/aeffguieditor.cpp \
	src/src/vstgui/vstgui.cpp src/src/ladspa/jLADSPAwRapperMAIN.cpp
	
	
clean:
	rm -f ./bin/*.so 		
	
