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


#ifndef __VSTV23ToPlug__
#include "VSTV23ToPlug.h"
#endif


extern VSTV23ToPlug* getWrapperInstance(JNIEnv *env, jobject obj);



//Impl of Plug -> Host calls
//Java -> Native
//impls for VSTV22ToHost.java native calls
//**********************************************