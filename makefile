# This is the LINUX makefile
# Other Platforms use IDE build-in features for compilation (Mac: XCode, Win: MS Visual Studio)
JAVA_HOME=/usr/lib/jvm/java-6-sun


all:	release

debug:
#	debug build option as executable so that all errors are reported!
#	g++ -o ./bin/jvstwrapper.so -O0 -g -Wall -rdynamic -ldl -I/usr/include -I./src/src/public.sdk/vst2.x -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux  src/src/jVSTwRapperMAIN.cpp

#	the real thing is here...
	g++ -o ./bin/jvstwrapper.so -shared -fPIC -ggdb -Wall -rdynamic -ldl -I/usr/include -I./src/src/public.sdk/vst2.x -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux  src/src/jVSTwRapperMAIN.cpp

release:
	g++ -o ./bin/jvstwrapper.so -shared -fPIC -O2 -Wall -rdynamic -ldl -I/usr/include -I./src/src/public.sdk/vst2.x -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux  src/src/jVSTwRapperMAIN.cpp
	
clean:
	rm -f ./bin/*.so 
	