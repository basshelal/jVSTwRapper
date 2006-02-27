//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Java -> Native calls impls
//-------------------------------------------------------------------------------------------------------


#ifndef _Included_jvst_wrapper_communication_VSTV22ToHost
#include "VSTV22ToHost.h"
#endif


#ifndef __JNIUtils__
#include "JNIUtils.h"
#endif


#ifndef __VSTV24ToPlug__
#include "VSTV24ToPlug.h"
#endif


extern VSTV24ToPlug* getWrapperInstance(JNIEnv *env, jobject obj);



//Impl of Plug -> Host calls
//Java -> Native
//impls for VSTV22ToHost.java native calls
//**********************************************