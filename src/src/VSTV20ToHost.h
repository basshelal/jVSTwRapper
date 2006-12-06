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

/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class jvst_wrapper_communication_VSTV20ToHost */

#ifndef _Included_jvst_wrapper_communication_VSTV20ToHost
#define _Included_jvst_wrapper_communication_VSTV20ToHost
#ifdef __cplusplus
extern "C" {
#endif
/* Inaccessible static: logBasePath */
/* Inaccessible static: logFileName */
/* Inaccessible static: logEnabled */
#undef jvst_wrapper_communication_VSTV20ToHost_CANDO_DONT_KNOW
#define jvst_wrapper_communication_VSTV20ToHost_CANDO_DONT_KNOW 0L
#undef jvst_wrapper_communication_VSTV20ToHost_CANDO_YES
#define jvst_wrapper_communication_VSTV20ToHost_CANDO_YES 1L
#undef jvst_wrapper_communication_VSTV20ToHost_CANDO_NO
#define jvst_wrapper_communication_VSTV20ToHost_CANDO_NO -1L
#undef jvst_wrapper_communication_VSTV20ToHost_VST_LANG_ENGLISH
#define jvst_wrapper_communication_VSTV20ToHost_VST_LANG_ENGLISH 1L
#undef jvst_wrapper_communication_VSTV20ToHost_VST_LANG_GERMAN
#define jvst_wrapper_communication_VSTV20ToHost_VST_LANG_GERMAN 2L
#undef jvst_wrapper_communication_VSTV20ToHost_VST_LANG_FRENCH
#define jvst_wrapper_communication_VSTV20ToHost_VST_LANG_FRENCH 3L
#undef jvst_wrapper_communication_VSTV20ToHost_VST_LANG_ITALIAN
#define jvst_wrapper_communication_VSTV20ToHost_VST_LANG_ITALIAN 4L
#undef jvst_wrapper_communication_VSTV20ToHost_VST_LANG_SPANISH
#define jvst_wrapper_communication_VSTV20ToHost_VST_LANG_SPANISH 5L
#undef jvst_wrapper_communication_VSTV20ToHost_VST_LANG_JAPANESE
#define jvst_wrapper_communication_VSTV20ToHost_VST_LANG_JAPANESE 6L
/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    canHostDo
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_canHostDo
  (JNIEnv *, jobject, jstring);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    sizeWindow
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_sizeWindow
  (JNIEnv *, jobject, jint,jint);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    ioChanged
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_ioChanged
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getTimeInfo
 * Signature: (I)Ljvst/wrapper/valueobjects/VSTTimeInfo;
 */
JNIEXPORT jobject JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getTimeInfo
  (JNIEnv *, jobject, jint);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    sendVstEventsToHost
 * Signature: (Ljvst/wrapper/valueobjects/VSTEvents;)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_sendVstEventsToHost
  (JNIEnv *, jobject, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    needIdle
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_needIdle
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    updateSampleRate
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_updateSampleRate
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    updateBlockSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_updateBlockSize
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getAutomationState
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getAutomationState
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getCurrentProcessLevel
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getCurrentProcessLevel
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getDirectory
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getDirectory
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getHostProductString
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getHostProductString
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getHostVendorVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getHostVendorVersion
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getHostVendorString
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getHostVendorString
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getHostLanguage
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getHostLanguage
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getInputLatency
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getInputLatency
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getOutputLatency
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getOutputLatency
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getNumAutomatableParameters
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getNumAutomatableParameters
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getParameterQuantization
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getParameterQuantization
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getSpeakerArrangement
 * Signature: (Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getSpeakerArrangement
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    hasExternalBuffer
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_hasExternalBuffer
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    isSynth
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_isSynth
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    noTail
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_noTail
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    setOutputSamplerate
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_setOutputSamplerate
  (JNIEnv *, jobject, jfloat);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    tempoAt
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_tempoAt
  (JNIEnv *, jobject, jint);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    updateDisplay
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_updateDisplay
  (JNIEnv *, jobject);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    wantAsyncOperation
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_wantAsyncOperation
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    wantEvents
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_wantEvents
  (JNIEnv *, jobject, jint);

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    willProcessReplacing
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_willProcessReplacing
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
