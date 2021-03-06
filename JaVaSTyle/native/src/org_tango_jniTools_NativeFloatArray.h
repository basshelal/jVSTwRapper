/* DO NOT EDIT THIS FILE - it is machine generated */
#include "jni.h"
/* Header for class org_tango_jniTools_NativeFloatArray */

#ifndef _Included_org_tango_jniTools_NativeFloatArray
#define _Included_org_tango_jniTools_NativeFloatArray
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_tango_jniTools_NativeFloatArray
 * Method:    initJNI
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_tango_jniTools_NativeFloatArray_initJNI
  (JNIEnv *, jclass);

/*
 * Class:     org_tango_jniTools_NativeFloatArray
 * Method:    get
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_org_tango_jniTools_NativeFloatArray_get
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_tango_jniTools_NativeFloatArray
 * Method:    copyToJava
 * Signature: ([F)Z
 */
JNIEXPORT jboolean JNICALL Java_org_tango_jniTools_NativeFloatArray_copyToJava___3F
  (JNIEnv *, jobject, jfloatArray);

/*
 * Class:     org_tango_jniTools_NativeFloatArray
 * Method:    copyFromJava
 * Signature: ([F)Z
 */
JNIEXPORT jboolean JNICALL Java_org_tango_jniTools_NativeFloatArray_copyFromJava___3F
  (JNIEnv *, jobject, jfloatArray);

/*
 * Class:     org_tango_jniTools_NativeFloatArray
 * Method:    copyToJava
 * Signature: ([FIII)Z
 */
JNIEXPORT jboolean JNICALL Java_org_tango_jniTools_NativeFloatArray_copyToJava___3FIII
  (JNIEnv *, jobject, jfloatArray, jint, jint, jint);

/*
 * Class:     org_tango_jniTools_NativeFloatArray
 * Method:    copyFromJava
 * Signature: ([FIII)Z
 */
JNIEXPORT jboolean JNICALL Java_org_tango_jniTools_NativeFloatArray_copyFromJava___3FIII
  (JNIEnv *, jobject, jfloatArray, jint, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
