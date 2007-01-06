/* 
 * jVSTwRapper - The Java way into VST world!
 * 
 * jVSTwRapper is an easy and reliable Java Wrapper for the Steinberg VST interface. 
 * It enables you to develop VST 2.3 compatible audio plugins and virtual instruments 
 * plus user interfaces with the Java Programming Language. 3 Demo Plugins(+src) are included!
 * 
 * Copyright (C) 2006  Daniel Martin [daniel309@users.sourceforge.net] 
 * 					   and many others, see CREDITS.txt
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Dll WinAPI entry
// VSTi API entry
//-
// 2003,2004 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------


#ifndef __jVSTwRapperMAIN__
#include "jVSTwRapperMAIN.h"
#endif

#ifndef __JNIUtils__
#include "JNIUtils.h"
#endif

#ifndef __VSTV24ToPlug__
#include "VSTV24ToPlug.h"
#endif

#ifndef __ConfigFileReader__
#include "ConfigFileReader.h"
#endif

#ifndef MACX
	#include <windows.h>

	extern jint (JNICALL *PTR_CreateJavaVM)(JavaVM **, void **, void *); 
	extern jint (JNICALL *PTR_GetCreatedJavaVMs)(JavaVM **, jsize, jsize *);
#else
	#include <Carbon/Carbon.h>
	#include <sys/stat.h>
	#include <sys/resource.h>
	#include <pthread.h>

	extern "C" {
		#include <mach-o/dyld.h>
		#include <mach-o/ldsyms.h>
	}
	#include <CoreFoundation/CFBundle.h>
	#include <CoreFoundation/CoreFoundation.h>
	
	//cocoa init stuff
	#include "VSTGUIWrapperMAC.h"
	
	//*** TODO: Set these defines accordingly before relaesing any files! ***
	//#define DEBUG
	//#define MACX_INIT_COCOA
#endif



//------------------------------------------------------------------------
extern int IsLogEnabled;

//------------------------------------------------------------------------
VSTV24ToPlug* WrapperInstance;

//------------------------------------------------------------------------
void calculatePaths();
AEffect* jvst_main(audioMasterCallback pAudioMaster);


#ifdef MACX
	CFTypeRef runLoop;

	void sourceCallBack(void *info);
	int loadCFApp();
	int startJavaThread();

	void* startJava(void *nix); 

	int checkJVMVersionRequest(char* requestedJVMVersion);
#else
	int startJava(void *nix);
#endif

//------------------------------------------------------------------------
char DllLocation[512];
char DllPath[512];

char ConfigFileName[100];
char LogFileName[100];
audioMasterCallback audioMaster;


//main entry points for different platforms
extern "C" {
#if defined (__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
	#define VST_EXPORT	__attribute__ ((visibility ("default")))
#else
	#define VST_EXPORT
#endif

VST_EXPORT AEffect* VSTPluginMain (audioMasterCallback pAudioMaster) {return jvst_main(pAudioMaster);}
//------------------------------------------------------------------------
#ifndef MACX
//disabled because vc++ 2005 does strict ansi c++ checks now (no other return type than int allowed for main!)
//nope, second try //old win main entry function
VST_EXPORT AEffect *MAIN (audioMasterCallback pAudioMaster) {return jvst_main(pAudioMaster);}
#else	
VST_EXPORT AEffect *main_macho (audioMasterCallback pAudioMaster) {return jvst_main(pAudioMaster);}
#endif
} // extern "C"



//-----------------------------------------------------------------------
//main entry point! --> The real thing
//-----------------------------------------------------------------------
AEffect* jvst_main(audioMasterCallback pAudioMaster) {
	calculatePaths();
	
	audioMaster=pAudioMaster;

	char log_location[700];
	strcpy(log_location, DllPath);
	strcat(log_location, LogFileName);

#ifndef DEBUG
	//redirecting system streams...
	FILE *err_stream = freopen(log_location, "a", stderr);
	if (err_stream!=NULL) log("\nredirecting stderr stream OK");
	FILE *out_stream = freopen(log_location, "a", stdout);
	if (out_stream!=NULL) log("redirecting stdout stream OK");
#else
	log("***** ACHTUNG: DEBUG=true ****");
#endif
	log("\n***** START *****");
	log(log_location);
	
	// Get VST Version
	if (!pAudioMaster(0, audioMasterVersion, 0, 0, 0, 0)) return 0;  // old version
	log("Get VST Version OK!");

	WrapperInstance = NULL;

	ConfigFileReader *cfg = new ConfigFileReader();
	
#ifndef MACX
	//see if there is a custom JVM we want to load (from the .ini file)!
	char* jvmLibLocation;
	jvmLibLocation = cfg->CustomJVMLocation;
	
	if (jvmLibLocation==NULL) {
		//no custom jvm specified, see if there is a request for a specific version in the .ini
		if (cfg->RequestedJVMVersion!=NULL) {
			log("Looking for a JVM version");
			log(cfg->RequestedJVMVersion);
			jvmLibLocation = readJVMLibLocation(cfg->RequestedJVMVersion);
			if(jvmLibLocation!=NULL) {
				log("desired JVM version found in registry at");
				log(jvmLibLocation);
			}
			else log("Could not find desired JVM version, seems not to be installed. Falling back to default JVM.");
		}
		
		if (jvmLibLocation==NULL) {
			log("querying registry for location of DEFAULT jvm.dll");
			//no custom JVM configured in the .ini. Look in the windows registry
			//get jvm location from registry and load jvm interface pointers
			jvmLibLocation = readJVMLibLocation(NULL);

			if (jvmLibLocation==NULL) {
				log("* WARNING: Could not find jvm.dll location in registry! \n using the one from the PATH environment variable!");
				//try loading jvm.dll from PATH
				jvmLibLocation = "jvm.dll";
			}
			else {
				log("found jvm.lib in registry at");
				log(jvmLibLocation);
			}
		}
	}
	else {
		log("Loading custom JVM !!! at");
		jvmLibLocation = replace(jvmLibLocation, "{WrapperPath}", DllPath);
		log(jvmLibLocation);
	}
	
	
	if (initJVMFunctionPointers(jvmLibLocation) != 0) {
		log("** ERROR: cant init jvm interface pointers!");
		return NULL;
	}
	
	//start the jvm
	if (startJava(NULL) != 0)  {
		log("**ERROR in startJava()");
		return NULL;
	}


#else
	//mac doesnt need all this custom jvm loading mess
	//it comes with a preinstalled jvm ;-), but we need to start he jvm in a separate thread
	
	//and we have the feature of requesting a specific jvm in the .ini file
	if (cfg->RequestedJVMVersion!=NULL) {
		int ret = -1;
		ret = checkJVMVersionRequest(cfg->RequestedJVMVersion);
		
		//make sure that we use any jvm
		if (ret!=0) {
			log("Warning: Could NOT find requested JVM version, using default JVM instead");
			unsetenv("JAVA_JVM_VERSION");
		} 
	}
	
	if (startJavaThread() != 0) {
		log("**ERROR in startJavaThread()");
		return NULL;
	}
	
#endif

	if (cfg) delete cfg;
	log("ALLES OK!"); 
	return WrapperInstance->getAeffect();
}

//------------------------------------------------------------------------
#ifndef MACX
int startJava(void *nix) {
#else
void* startJava(void *nix) {
#endif
	//Create JVM
	//**********************************************
	JNIEnv *env;
	JavaVM *jvm;
	jint res;
	int result;
	JavaVMInitArgs vm_args;
	JavaVMOption options[6]; //assume the max number of options...
	char java_path[1024]; //we need to add jVSTsYstem_bin.jar to the ClassPath of the Bootstrap ClassLoader!
	char class_path[1024];

	
	result = -1;
	
	ConfigFileReader *cfg = new ConfigFileReader();
	IsLogEnabled = cfg->IsLoggingEnabled;
	
	log("DllPath (implicitly added to the classpath)");
	log(DllPath);
	
	strcpy(class_path, DllPath);
#ifdef MACX
	strcat(class_path, ":\0");
#else
	strcat(class_path, ";\0");
#endif
	strcat(class_path, replace(cfg->JVMClassPath, "{WrapperPath}", DllPath));


	strcpy(java_path, "-Djava.class.path=");
	strcat(java_path, replace(cfg->SystemClassPath, "{WrapperPath}", DllPath));

	log("SystemPath");
	log(java_path);
	log("VSTiClassLoader Path");
	log(class_path);

	options[0].optionString = java_path;
	
	//If the mac still causes problems even when not using a java swing gui
	//try running the jvm in "headless" mode (so that cocoa doesnt even get initialised)
	//-Djava.awt.headless=true
	
	//apply the options from the .ini file
	int optionNum = 0;	
	if (cfg->JVMOption1!=NULL) {
		optionNum++;
		log("Using additional JVM option");
		log(cfg->JVMOption1);
		options[optionNum].optionString=cfg->JVMOption1;
	}
	if (cfg->JVMOption2!=NULL) {
		optionNum++;
		log("Using additional JVM option");
		log(cfg->JVMOption2);
		options[optionNum].optionString=cfg->JVMOption2;
	}
	if (cfg->JVMOption3!=NULL) {
		optionNum++;
		log("Using additional JVM option");
		log(cfg->JVMOption3);
		options[optionNum].optionString=cfg->JVMOption3;
	}
	if (cfg->JVMOption4!=NULL) {
		optionNum++;
		log("Using additional JVM option");
		log(cfg->JVMOption4);
		options[optionNum].optionString=cfg->JVMOption4;
	}
	if (cfg->JVMOption5!=NULL) {
		optionNum++;
		log("Using additional JVM option");
		log(cfg->JVMOption5);
		options[optionNum].optionString=cfg->JVMOption5;
	}


	optionNum++;
#ifndef MACX
	vm_args.version = JNI_VERSION_1_2;
#else
	vm_args.version = JNI_VERSION_1_4; //we want at least 1.4 on the mac!
#endif
	vm_args.options = options;
	vm_args.nOptions = optionNum;
	vm_args.ignoreUnrecognized = JNI_FALSE; //use -X options as well!

	//erst schauen ob schon ne jvm aktiv ist:
	JavaVM *vmBuffer;
	jsize nVMs;

	bool hasGUI = false;
	jclass guiClass = NULL;

#ifndef MACX
	res = PTR_GetCreatedJavaVMs(&vmBuffer, 1, &nVMs);
#else
	res = JNI_GetCreatedJavaVMs(&vmBuffer, 1, &nVMs);
#endif
	if (res < 0) {
		log("** ERROR: Can't get created Java VMs");
		goto leave;
	}

	if (nVMs>0) {
		log("before reusing JavaVM");
		jvm = &vmBuffer[nVMs-1];
		res = jvm->GetEnv((void**)&env, JNI_VERSION_1_2);
		log("after reusing JavaVM");
		
		log("* Warning: Since we are reusing a Java VM which is already in memory, \n\
			all VM startup options will be ignored! You need to restart your audio app in order to make them effective!");

		if (res==JNI_EDETACHED) { 
			log("attaching current thread to jvm!");
			res = jvm->AttachCurrentThread((void**)&env, NULL);
			if (res<0) log ("** ERROR: Attaching JVM to native thread!");
		}
		
		if (res < 0) {
			log("** ERROR: getting Java env!");
			if (res==JNI_EVERSION) log("GetEnv Error because of different JNI Version!");
			goto leave;
		}
	} 
	else {
		log("before JNI_CreateJavaVM");
		/* Create the Java VM */
#ifndef MACX
	res = PTR_CreateJavaVM(&jvm, (void**)&env, &vm_args);
#else
	res = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
#endif		
		log("AFTER JNI_CreateJavaVM");
		
		if (res < 0) {
			log("** ERROR: Can't create Java VM (are your VM options in the .ini file correct?)");
			goto leave;
		}
		if (checkException(env)) goto leave;
	}
	//***********************************************

	//IMPORTANT: clear possible pending exceptions...
	if (checkException(env)) goto leave;

	//USE THE VSTiClassLoaderManager here!!!
	//load this ClassLoader with the bootstrap ClassLoader
	jclass manager = env->FindClass("jvst/wrapper/system/VSTiClassLoaderManager");
	if (manager == NULL) {
		log("** ERROR: cannot find jvst/wrapper/system/VSTiClassLoaderManager");
		checkException(env); //print statck trace...
		goto leave;
	}

	jmethodID loadcl_mid = env->GetStaticMethodID(manager, "loadVSTiClass", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Class;");
	if (loadcl_mid == NULL)  {
		log("** ERROR: cannot find static method loadVSTiClass(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Class;");
		checkException(env); //print statck trace...
		goto leave;
	}

	jstring dllloc = env->NewStringUTF(DllLocation);
	jstring plug = env->NewStringUTF(cfg->PluginClass);
	jstring cp = env->NewStringUTF(class_path);
	jclass clazz = (jclass)env->CallStaticObjectMethod(manager, loadcl_mid, dllloc, plug, cp);
	if (checkException(env)) goto leave;


	//calling static _initPlugFromNative
	//passing config data
	jmethodID init_mid = env->GetStaticMethodID(clazz, "_initPlugFromNative", "(Ljava/lang/String;Z)V");
	if (init_mid == NULL)  {
		log("** ERROR: cannot find static method _initPlugFromNative(Ljava/lang/String;Z)V");
		checkException(env); //print statck trace...
		goto leave;
	}
	jstring dll = env->NewStringUTF(DllLocation);
	jboolean doLogging = cfg->IsLoggingEnabled;
	env->CallStaticVoidMethod(clazz, init_mid, dll, doLogging);

	if (checkException(env)) goto leave;


	// Create the AudioEffect
	log("before Wrapper Constructor");
	WrapperInstance = new VSTV24ToPlug(audioMaster, 1, 1, jvm);
	if (!WrapperInstance) {
		log("** ERROR: Error Creating VST Wrapper instance");
		delete WrapperInstance;
		checkException(env); //print statck trace...
		goto leave;
	}

	//test if we can load the GUI class
	if (cfg->PluginUIClass!=NULL) {
		log("loding gui class");
		log(cfg->PluginUIClass);

		//load gui using our own ClassLoader...
		jstring gui = env->NewStringUTF(cfg->PluginUIClass);

		guiClass = (jclass)env->CallStaticObjectMethod(manager, loadcl_mid, dllloc, gui, cp);
		if (checkException(env)) {
			guiClass=NULL;
			hasGUI=false;
			log("* WARNING: Could not load GUI class (wrong name in .ini or exception in constructor?). Using Plugs default UI!");
		}
		else hasGUI = true;
	}
	
	//calling Java side constructors
	log("calling effects java construtor!");
	if (WrapperInstance->initJavaSide(clazz, hasGUI)) goto leave;

	if (checkException(env)) goto leave;


	//calling java guis init!
	if((WrapperInstance->getEditor() != NULL) && hasGUI) {
		if (((VSTGUIWrapper*)WrapperInstance->getEditor())->initJavaSide(guiClass)) goto leave;
	}

	if (checkException(env)) goto leave;

	delete cfg;
	result = NULL; //everythign is fine when we came to this point

leave:
#ifdef MACX
	//TODO: ASK Gerard why we need this ???
	CFRunLoopStop((CFRunLoopRef)runLoop);
	return NULL; //NULL //man, this makes the whole result var unecessary...
#else
	return result;
#endif
}

//------------------------------------------------------------------------

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
	return JNI_VERSION_1_2; //we use JNI 1.2 functions, tell that the jvm that loads us. 
							//we dont need jni 1.4 functionality, so 1.2 is enough
}

//------------------------------------------------------------------------
void calculatePaths() {

#ifndef MACX
	//calculating paths
	char* lastSlash = strrchr(DllLocation, '\\');
	
	//config file name
	strcpy(ConfigFileName, lastSlash);
	char* lastPoint = strrchr(ConfigFileName, '.');
	int len = lastPoint - ConfigFileName;
	strncpy(ConfigFileName + len + 1, "ini\0", 4);
	
	//log file name
	strcpy(LogFileName, lastSlash);
	lastPoint = strrchr(LogFileName, '.');
	len = lastPoint - LogFileName;
	strncpy(LogFileName + len, "_log.txt\0", 9);
	
	//DllPath
	len = lastSlash - DllLocation;
	strncpy(DllPath, DllLocation, len);
	DllPath[len]='\0';
	
#else

	const mach_header* header = &_mh_bundle_header;
	const char* imagename = 0;
	int cnt = _dyld_image_count();
	for (int idx1 = 1; idx1 < cnt; idx1++) {
		if (_dyld_get_image_header((unsigned long)idx1) == header) {			
			imagename = _dyld_get_image_name(idx1);
			break;
		}
	}
	if (imagename == 0) {
		log("** ERROR: Cant read image name from bundle!");
		return;
	}
	
	char* lastSlash = strrchr(imagename, '/') + 1;
	
	int len = strlen(lastSlash);
	strcpy(DllLocation, imagename);
	printf("DllLocation=%s\n", DllLocation);
	
	//config file name
	strcpy(ConfigFileName, lastSlash);
	strncpy(ConfigFileName + len , ".ini\0", 5);
	printf("ConfigFileName=%s\n", ConfigFileName);
	
	//log file name
	strcpy(LogFileName, lastSlash);
	strncpy(LogFileName + len, "_log.txt\0", 9);
	printf("LogFileName=%s\n", LogFileName);

	//DllPath
	len = lastSlash - imagename - 1;
	char tmp[1024];
	strncpy(tmp, imagename, len);
	tmp[len]='\0';
	lastSlash = strrchr(tmp, '/');
	len = lastSlash - tmp;
	strncpy(DllPath, imagename, len);
	DllPath[len]='\0';
	strcat(DllPath,"/Resources/\0");
	printf("DllPath=%s\n", DllPath);
#endif
	
}


#ifndef MACX

//------------------------------------------------------------------------
// WINAPI Dll Entry
//------------------------------------------------------------------------
void* hInstance;

BOOL WINAPI DllMain (HINSTANCE hInst, DWORD dwReason, LPVOID lpvReserved) {
	hInstance = hInst;

	GetModuleFileName(hInst, DllLocation, 512);
	return 1;
}



#else

void sourceCallBack (  void *info  ) {}

int startJavaThread(){
	log("starting java thread");

#ifdef MACX_INIT_COCOA
	//init cocoa to be able to interop with it in carbon
	if (initializeCocoa()==0) log("Cocoa initialized successfully!");
	else {
		log("** Error while initilizing Cocoa");
		return -1;
	}
#else
	log("NOT using Cocoa!");
#endif

	/* Start the thread that runs the VM. */
	CFRunLoopSourceContext sourceContext;
	pthread_t vmthread;
	
	/* create a new pthread copying the stack size of the primordial pthread */ 
	struct rlimit limit;
    size_t stack_size = 0;
    int rc = getrlimit(RLIMIT_STACK, &limit);
    if (rc == 0) {
        if (limit.rlim_cur != 0LL) {
            stack_size = (size_t)limit.rlim_cur;
        }
    }
    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setscope(&thread_attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    if (stack_size > 0) {
        pthread_attr_setstacksize(&thread_attr, stack_size);
    }	

    /* Start the thread that we will start the JVM on. */
    rc = pthread_create(&vmthread, &thread_attr, &startJava, NULL);
	if (rc != 0) {
		log("** Error creating thread via pthread_create() !");
		return -1;
	}
	pthread_attr_destroy(&thread_attr);
    
	/* Create a a sourceContext to be used by our source that makes */
    /* sure the CFRunLoop doesn't exit right away */
	sourceContext.version = 0;
    sourceContext.info = NULL;
    sourceContext.retain = NULL;
    sourceContext.release = NULL;
    sourceContext.copyDescription = NULL;
    sourceContext.equal = NULL;
    sourceContext.hash = NULL;
    sourceContext.schedule = NULL;
    sourceContext.cancel = NULL;
    sourceContext.perform = &sourceCallBack;
	
	/* Create the Source from the sourceContext */
	CFRunLoopSourceRef sourceRef = CFRunLoopSourceCreate (NULL, 0, &sourceContext); 
	
	/* Use the constant kCFRunLoopCommonModes to add the source to the set of objects */ 
    /* monitored by all the common modes */
    CFRunLoopAddSource (CFRunLoopGetCurrent(),sourceRef,kCFRunLoopCommonModes); 	
	
	//get ref to stop the run loop later on (at the end of startJava())
	//EventLoopRef eventLoop = GetCurrentEventLoop ();
	//runLoop=GetCFRunLoopFromEventLoop(eventLoop);	
    runLoop = CFRunLoopGetCurrent(); //This one fixed the infite block with the Amadeus II host!
	
	/* Park this thread in the runloop */
	CFRunLoopRun();		

	return 0;
}

#endif

