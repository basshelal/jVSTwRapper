//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Java -> Native calls impls
//-------------------------------------------------------------------------------------------------------


#ifndef _Included_jvst_wrapper_communication_VSTV23ToHost
#include "VSTV23ToHost.h"
#endif

#ifndef __JNIUtils__
#include "JNIUtils.h"
#endif

#ifndef __VSTV23ToPlug__
#include "VSTV23ToPlug.h"
#endif


extern VSTV23ToPlug* WrapperInstance;



//Impl of Plug -> Host calls
//Java -> Native
//impls for VSTV23ToHost.java native calls
//**********************************************



/*
 * Class:     jvst_wrapper_communication_VSTV23ToHost
 * Method:    setPanLaw
 * Signature: (IF)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV23ToHost_setPanLaw
  (JNIEnv* env, jobject obj, jint type, jfloat val) {

	  if (WrapperInstance!=NULL) return WrapperInstance->setPanLaw(type, val);
	  else return 0;
}

/*
 * Class:     jvst_wrapper_communication_VSTV23ToHost
 * Method:    beginLoadBank
 * Signature: (Ljvst/wrapper/valueobjects/VSTPatchChunkInfo;)I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV23ToHost_beginLoadBank
  (JNIEnv *env, jobject obj, jobject bank) {

	  return 0;
}

/*
 * Class:     jvst_wrapper_communication_VSTV23ToHost
 * Method:    beginLoadProgram
 * Signature: (Ljvst/wrapper/valueobjects/VSTPatchChunkInfo;)I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV23ToHost_beginLoadProgram
	(JNIEnv *env, jobject obj, jobject prog) {

		return 0;
}

/*
 * Class:     jvst_wrapper_communication_VSTV23ToHost
 * Method:    copySpeaker
 * Signature: (Ljvst/wrapper/valueobjects/VSTSpeakerProperties;Ljvst/wrapper/valueobjects/VSTSpeakerProperties;)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV23ToHost_copySpeaker
  (JNIEnv *env, jobject obj, jobject from, jobject to) {

	return 0;
}

/*
 * Class:     jvst_wrapper_communication_VSTV23ToHost
 * Method:    matchArrangement
 * Signature: (Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV23ToHost_matchArrangement
  (JNIEnv *env, jobject obj, jobject obj1, jobject obj2) {

  return 0;
}

