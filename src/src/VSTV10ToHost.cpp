 //-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Java -> Native calls impls
//-
// 2003,2004 Daniel Martin
//-------------------------------------------------------------------------------------------------------


#ifndef _Included_jvst_wrapper_communication_VSTV10ToHost
#include "VSTV10ToHost.h"
#endif

#ifndef __JNIUtils__
#include "JNIUtils.h"
#endif

#ifndef __VSTV23ToPlug__
#include "VSTV23ToPlug.h"
#endif


VSTV23ToPlug* getWrapperInstance(JNIEnv *env, jobject obj)
{
	jclass plugclass=env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(plugclass, "WrapperInstance", "J");
	if (fid == NULL) {
		log("** ERROR: cannot find effects WrappeInstance");		
		checkAndThrowException(env);
		return 0;
	}
	jlong wri=env->GetLongField(obj,fid);	
	if (checkAndThrowException(env)) return 0;
	return (VSTV23ToPlug*)wri;
}



//Impl of Plug -> Host calls
//Java -> Native
//**********************************************



//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    setUniqueID
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_setUniqueID(JNIEnv *env, jobject obj, jint id) {
	log("setUniqueID");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->setUniqueID((long)id);
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    setNumInputs
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_setNumInputs(JNIEnv *env, jobject obj, jint num) {
	log("setNumInputs");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->setNumInputs((long)num);
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    setNumOutputs
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_setNumOutputs(JNIEnv *env, jobject obj, jint num) {
	log("setNumOutputs");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL)WrapperInstance->setNumOutputs((long)num);
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    hasVu
 * Signature: ()V
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_hasVu(JNIEnv *env, jobject obj, jboolean state) {
	log("hasVu");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->hasVu(state != 0 ? true : false);
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    hasClip
 * Signature: ()Z
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_hasClip(JNIEnv *env, jobject obj, jboolean state) {
	log("hasClip");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->hasClip(state != 0 ? true : false);
}


//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    canMono
 * Signature: ()Z
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_canMono(JNIEnv *env, jobject obj, jboolean state) {
	log("canMono");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->canMono(state != 0 ? true : false);
}


//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    canProcessReplacing
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_canProcessReplacing(JNIEnv *env, jobject obj, jboolean state) {
	bool test = state != 0 ? true : false;
	char dump[100];
	sprintf(dump, "canProcessReplacing=%i", test);
	log(dump);
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->canProcessReplacing(state != 0 ? true : false);
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    setRealtimeQualities
 * Signature: (I)V
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_setRealtimeQualities(JNIEnv *env, jobject obj, jint quality) {
	log("setRealtimeQualities");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->setRealtimeQualities(quality);
}



//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    setOfflineQualities
 * Signature: (I)V
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_setOfflineQualities(JNIEnv *env, jobject obj, jint quality) {
	log("setOfflineQualities");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->setOfflineQualities(quality);
}


//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    setInitialDelay
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_setInitialDelay(JNIEnv *env, jobject obj, jint delay) {
	log("setInitialDelay");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->setInitialDelay(delay);
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    programsAreChunks
 * Signature: ()Z
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_programsAreChunks(JNIEnv *env, jobject obj, jboolean state) {
	log("programsAreChunks");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->programsAreChunks(state != 0 ? true : false);
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    getSampleRate
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_getSampleRate(JNIEnv *env, jobject obj) {
	log("getSampleRate");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getSampleRate();
	else return -1.0F;
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    getBlockSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_getBlockSize(JNIEnv *env, jobject obj) {
	log("getBlockSize");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getBlockSize();
	else return 0;
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    setParameterAutomated
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_setParameterAutomated(JNIEnv *env, jobject obj, jint index, jfloat value) {
	log("setParameterAutomated");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->setParameterAutomated(index, value);
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    getMasterVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_getMasterVersion(JNIEnv *env, jobject obj) {
	log("getMasterVersion");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getMasterVersion();
	else return -1;
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    getCurrentUniqueId
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_getCurrentUniqueId(JNIEnv *env, jobject obj) {
	log("getCurrentUniqueId");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getCurrentUniqueId();
	else return -1;
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    masterIdle
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_masterIdle(JNIEnv *env, jobject obj) {
	log("masterIdle");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->masterIdle();
}


//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    isInputConnected
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_isInputConnected(JNIEnv *env, jobject obj, jint input) {
	log("isInputConnected");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->isInputConnected(input);
	else return false;
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    isOutputConnected
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_isOutputConnected(JNIEnv *env, jobject obj, jint output) {
	log("isOutputConnected");
	VSTV23ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->isOutputConnected(output);
	else return false;
}







//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    sayHello
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_sayHello(JNIEnv *, jobject) {
	printf("\nC++ sagt Hallo!");
}
