//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Java -> Native calls impls
//-------------------------------------------------------------------------------------------------------


#ifndef _Included_jvst_wrapper_communication_VSTV21ToHost
#include "VSTV21ToHost.h"
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
//impls for VSTV21ToHost.java native calls
//**********************************************



/*
 * Class:     jvst_wrapper_communication_VSTV21ToHost
 * Method:    beginEdit
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV21ToHost_beginEdit
	(JNIEnv *env, jobject obj, jint index) {
        VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj); 
		if (WrapperInstance!=NULL) return WrapperInstance->beginEdit(index);
		else return 0;
}

/*
 * Class:     jvst_wrapper_communication_VSTV21ToHost
 * Method:    endEdit
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV21ToHost_endEdit
  (JNIEnv *env, jobject obj, jint index) {
        VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
		if (WrapperInstance!=NULL) return WrapperInstance->endEdit(index);
		else return 0;
}

