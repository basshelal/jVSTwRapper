//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Java -> Native calls impls
//-------------------------------------------------------------------------------------------------------


#ifndef _Included_jvst_wrapper_communication_VSTV24ToHost
#include "VSTV24ToHost.h"
#endif

#ifndef __JNIUtils__
#include "JNIUtils.h"
#endif

#ifndef __VSTV24ToPlug__
#include "VSTV24ToPlug.h"
#endif


extern VSTV23ToPlug* getWrapperInstance(JNIEnv *env, jobject obj);



//Impl of Plug -> Host calls
//Java -> Native
//impls for VSTV24ToHost.java native calls
//**********************************************



/*
 * Class:     jvst_wrapper_communication_VSTV24ToHost
 * Method:    canDoubleReplacing
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV24ToHost_canDoubleReplacing
	(JNIEnv *env, jobject obj, jboolean value) {

      VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	  if (WrapperInstance!=NULL) WrapperInstance->canDoubleReplacing(value != 0 ? true : false);
}
