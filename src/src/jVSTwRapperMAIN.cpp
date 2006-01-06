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

#ifndef __VSTV23ToPlug__
#include "VSTV23ToPlug.h"
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
#endif



//------------------------------------------------------------------------
extern int IsLogEnabled;

//------------------------------------------------------------------------
//VSTV23ToPlug* WrapperInstance;

//------------------------------------------------------------------------
void calculatePaths();
VSTV23ToPlug* startJava();


#ifdef MACX
	CFTypeRef runLoop;

	void sourceCallBack(void *info);
	void loadCFApp();
	void startJavaThread();
#endif

//------------------------------------------------------------------------
char DllLocation[512];
char DllPath[512];

char ConfigFileName[100];
char LogFileName[100];
audioMasterCallback audioMaster;


//------------------------------------------------------------------------
#ifndef MACX
AEffect *main (audioMasterCallback pAudioMaster) {
#else	
AEffect *main_macho (audioMasterCallback pAudioMaster) {	
	pthread_t *newThread;
#endif

	calculatePaths();
	audioMaster=pAudioMaster;
	char log_location[700];
	strcpy(log_location, DllPath);
	strcat(log_location, LogFileName);


	//redirecting system streams...
	FILE *err_stream = freopen(log_location, "a", stderr);
	if (err_stream!=NULL) log("\nredirecting stderr stream OK");
	FILE *out_stream = freopen(log_location, "a", stdout);
	if (out_stream!=NULL) log("redirecting stdout stream OK");
	
	log("\n***** START *****");
	log(log_location);
	
	// Get VST Version
	if (!pAudioMaster(0, audioMasterVersion, 0, 0, 0, 0)) return 0;  // old version
	log("Get VST Version OK!");

#ifndef MACX
	//get jvm location from registry and load jvm interface pointers
	char* jvmLibLocation = readJVMLibLocation();
	if (jvmLibLocation==NULL) {
		log("* WARNING: Could not find jvm.dll location in registry!");
		//try loading jvm.dll from PATH
		jvmLibLocation = "jvm.dll";
	}

	if (initJVMFunctionPointers(jvmLibLocation) != 0) {
		log("** ERROR: cant init jvm interface pointers!");
		return 0;
	}
    VSTV23ToPlug* WrapperInstance = startJava();
	//start the jvm
	if (WrapperInstance == NULL) {
		log("**ERROR in startJava()");
		return 0;
	}
#else
	startJavaThread();
#endif

	log("ALLES OK!");

	return WrapperInstance->getAeffect();
}


VSTV23ToPlug* startJava() {

	//Create JVM
	//**********************************************
	JNIEnv *env;
	JavaVM *jvm;
	jint res;
	JavaVMInitArgs vm_args;
	JavaVMOption options[6]; //assume the max number of options...
	char java_path[1024]; //we need to add jVSTsYstem_bin.jar to the ClassPath of the Bootstrap ClassLoader!
	char class_path[1024];

	VSTV23ToPlug* WrapperInstance;

	ConfigFileReader *cfg = new ConfigFileReader();
	IsLogEnabled = cfg->IsLoggingEnabled;
	
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

	vm_args.version = JNI_VERSION_1_2;
	vm_args.options = options;
	vm_args.nOptions = optionNum;
	vm_args.ignoreUnrecognized = JNI_FALSE; //use -X options as well!

	//erst schauen ob schon ne jvm aktiv ist:
	JavaVM *vmBuffer;
	jsize nVMs;

#ifndef MACX
	res = PTR_GetCreatedJavaVMs(&vmBuffer, 1, &nVMs);
#else
	res = JNI_GetCreatedJavaVMs(&vmBuffer, 1, &nVMs);
#endif
	if (res < 0) {
		log("** ERROR: Can't get created Java VMs");
		return 0;
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
			return 0;
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
			log("** ERROR: Can't create Java VM (are your VM options correct?)");
			return 0;
		}
		if (checkException(env)) return 0;
	}
	//***********************************************

	//IMPORTANT: clear possible pending exceptions...
	if (checkException(env)) return 0;

	/* Get a reference to obj's class */


	//USE THE VSTiClassLoaderManager here!!!
	//load this ClassLoader with the bootstrap ClassLoader
	jclass manager = env->FindClass("jvst/wrapper/system/VSTiClassLoaderManager");
	if (manager == NULL) {
		log("** ERROR: cannot find jvst/wrapper/system/VSTiClassLoaderManager");
		checkException(env); //print statck trace...
		return 0;
	}

	jmethodID loadcl_mid = env->GetStaticMethodID(manager, "loadVSTiClass", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Class;");
	if (loadcl_mid == NULL)  {
		log("** ERROR: cannot find static method loadVSTiClass(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Class;");
		checkException(env); //print statck trace...
		return 0;
	}

	jstring dllloc = env->NewStringUTF(DllLocation);
	jstring plug = env->NewStringUTF(cfg->PluginClass);
	jstring cp = env->NewStringUTF(class_path);
	jclass clazz = (jclass)env->CallStaticObjectMethod(manager, loadcl_mid, dllloc, plug, cp);
	if (checkException(env)) return 0;


	//calling static _initPlugFromNative
	//passing config data
	jmethodID init_mid = env->GetStaticMethodID(clazz, "_initPlugFromNative", "(Ljava/lang/String;Z)V");
	if (init_mid == NULL)  {
		log("** ERROR: cannot find static method _initPlugFromNative(Ljava/lang/String;Z)V");
		checkException(env); //print statck trace...
		return 0;
	}
	jstring dll = env->NewStringUTF(DllLocation);
	jboolean doLogging = cfg->IsLoggingEnabled;
	env->CallStaticVoidMethod(clazz, init_mid, dll, doLogging);

	if (checkException(env)) return 0;


	// Create the AudioEffect
	log("before Wrapper Constructor");
	WrapperInstance = new VSTV23ToPlug(audioMaster, 1, 1, jvm);
	if (!WrapperInstance) {
		log("** ERROR: Error Creating VST Wrapper instance");
		delete WrapperInstance;
		checkException(env); //print statck trace...
		return 0;
	}

	//test if we can load the GUI class
	bool hasGUI = false;
	jclass guiClass = NULL;
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
	if (WrapperInstance->initJavaSide(clazz, hasGUI)) return 0;

	if (checkException(env)) return 0;



	//calling java guis init!
	if((WrapperInstance->getEditor() != NULL) && hasGUI) {
		if (((VSTGUIWrapper*)WrapperInstance->getEditor())->initJavaSide(guiClass)) return 0;
	}

	if (checkException(env)) return 0;

	delete cfg;

#ifdef MACX
	CFRunLoopStop((CFRunLoopRef)runLoop);
#endif

	return WrapperInstance; //everythings fine...
}

//------------------------------------------------------------------------

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
	return JNI_VERSION_1_2;
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
	for (int idx1 = 1; idx1 < cnt; idx1++) 
	{
		if (_dyld_get_image_header((unsigned long)idx1) == header)
		{			
			imagename = _dyld_get_image_name(idx1);
			break;
		}
	}
	if (imagename == 0)
		return;
	
	char* lastSlash = strrchr(imagename, '/');
	
	//dll file name
	//hi gerard, you might get a compile error here ;-) (I deleted the var DllFileName, 
	//because we now use System.load("absolute native lib location") instead of System.loadLibrary(...))
	//you might need to modify your code to provide the correct value for DllLocation... sorry!
	strcpy(DllFileName, ++lastSlash);
	int len = strlen(lastSlash);
	strncpy(DllFileName + len , ".jnilib\0", 7);

	//config file name
	strcpy(ConfigFileName, lastSlash);
	char* lastPoint = strrchr(ConfigFileName, '.');
	len = strlen(ConfigFileName); 
	strncpy(ConfigFileName + len , ".INI\0", 4); // add dot
	
	//log file name
	strcpy(LogFileName, lastSlash);
	strncpy(LogFileName + len, "_log.txt\0", 9);

	//DllPath
	len = lastSlash - imagename -1;
	char tmp[1024];
	strncpy(tmp, imagename, len);
	lastSlash = strrchr(tmp, '/');
	len = lastSlash - tmp;
	strncpy(DllPath, imagename, len);
	DllPath[len]='\0';
	strcat(DllPath,"/Resources/\0");
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


//gerard, it might be a good idea to return an int from this 
//method to provide an indicator if the initialisation failed or not. 
//so that main_macho can fail cleanly, return 0 and doessnt crash the host.
void startJavaThread(){
	log("starting java thread ");
	loadCFApp();
	CFRunLoopSourceContext sourceContext;
	pthread_t vmthread;
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

    pthread_create(&vmthread, &thread_attr, &startJava, NULL);
    pthread_attr_destroy(&thread_attr);
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
	CFRunLoopSourceRef sourceRef = CFRunLoopSourceCreate (NULL, 0, &sourceContext);    
    CFRunLoopAddSource (CFRunLoopGetCurrent(),sourceRef,kCFRunLoopCommonModes); 	
	EventLoopRef eventLoop = GetCurrentEventLoop ();
	runLoop=GetCFRunLoopFromEventLoop(eventLoop);	
    CFRunLoopRun();		

}



void loadCFApp()
{
    CFBundleRef myAppsBundle= NULL;
	CFURLRef    bundleURL   = NULL;	
	OSStatus osStatus = noErr;
	OSStatus (*funcPtr)();
	Boolean isDir = true;
	int res;
	bundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,CFSTR("/System/Library/Frameworks/AppKit.framework"),kCFURLPOSIXPathStyle,isDir);	
	myAppsBundle = CFBundleCreate(NULL, bundleURL);
	
	funcPtr = CFBundleGetFunctionPointerForName(myAppsBundle,CFSTR("NSApplicationLoad"));
	log("loading cocoa");
	res = (int)funcPtr();
	if(!res)
	{
		log("***ERROR Unable to call NSApplication load in loadCFApp");
	}
}
#endif

