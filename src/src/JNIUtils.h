//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// JNI utility functions
//-
// 2003,2004 Daniel Martin
//-------------------------------------------------------------------------------------------------------

#ifndef __JNIUtils__
#define __JNIUtils__


#ifndef __audioeffectx__
#include "vst/audioeffectx.h"
#endif

#include <jni.h>



jvalue JNU_CallJavaMethod(	JNIEnv *env,
							jobject obj, 
							jmethodID mid, 
							const char *name,
							const char *descriptor, 
							...);

int log(char* c);
char *replace(char *string, char *oldpiece, char *newpiece);
bool checkException(JNIEnv *env);
bool checkAndThrowException(JNIEnv *env);


#ifndef MACX
	//stuff to load the jvm.dll dynamically
	//this removes the need to add the path to jvm.dll to the PATH variable
	int initJVMFunctionPointers(char *vmlibpath);
	char* readJVMLibLocation();
#endif



#endif