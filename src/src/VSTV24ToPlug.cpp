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

//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// A JNI Wrapper 
// for Java as the language to write VSTi Plugins
//-------------------------------------------------------------------------------------------------------

#include "VSTV24ToPlug.h"
#include "JNIUtils.h"


//Impl of Host -> Plug calls
//Native -> Java 
//-----------------------------------------------------------------------------
VSTV24ToPlug::VSTV24ToPlug (audioMasterCallback audioMaster, int progs, int parms, JavaVM *jvm) 
	: VSTV23ToPlug (audioMaster, progs, parms, jvm) {

	this->ProcessDoubleReplacingMethodID = NULL;
	//this->ProcessDoubleReplacingJEnv = NULL;
	//this->ProcessDoubleReplacingThreadID = 0;

	this->JavaDoubleClass = NULL;
}

//-----------------------------------------------------------------------------------------
VSTV24ToPlug::~VSTV24ToPlug () {

}


//-----------------------------------------------------------------------------------------
bool VSTV24ToPlug::setProcessPrecision (VstInt32 precision) {	
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "setProcessPrecision", "(I)Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method setProcessPrecision(I)Z"); return false;}
	
	jboolean ret = env->CallBooleanMethod(this->JavaPlugObj, mid, precision);
	
	if (this->checkException(env)) return false;

	return ret!=0;
} 

//-----------------------------------------------------------------------------------------
VstInt32 VSTV24ToPlug::getNumMidiInputChannels() {	
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getNumMidiInputChannels", "()I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getNumMidiInputChannels()I"); return false;}
	
	jint ret = env->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException(env);

	return ret;
} 			

//-----------------------------------------------------------------------------------------
VstInt32 VSTV24ToPlug::getNumMidiOutputChannels() {	
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getNumMidiOutputChannels", "()I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getNumMidiOutputChannels()I"); return false;}
	
	jint ret = env->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException(env);

	return ret;
} 

//-----------------------------------------------------------------------------------------
void VSTV24ToPlug::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames) {
/*
#ifdef WIN32
	DWORD threadID;
	threadID = GetCurrentThreadId();
	if (this->ProcessDoubleReplacingThreadID != threadID) {
		this->ProcessDoubleReplacingThreadID = threadID;
#endif
#if defined(MACX) || defined(linux)
		pthread_t threadID;
		threadID = pthread_self();
		if (!pthread_equal(threadID,this->ProcessDoubleReplacingThreadID)){
			this->ProcessDoubleReplacingThreadID = threadID;
			
#endif	
	
		jint stat = this->Jvm->AttachCurrentThread((void**)&this->ProcessDoubleReplacingJEnv, NULL);
		if (stat<0) log("** ERROR: attaching to .processDoubleReplacing() THREAD");

		log("ProcessDoubleReplacing ThreadID=%i", this->ProcessDoubleReplacingThreadID);
	}
*/

	JNIEnv* env = this->ensureJavaThreadAttachment();

	if(this->JavaDoubleClass == NULL) {
		this->JavaDoubleClass = env->FindClass("[D");
		if (this->JavaDoubleClass == NULL) log("** ERROR: cannot find class [D");
	}

	jobjectArray  jinputs;
	jobjectArray  joutputs;

	jinputs = env->NewObjectArray(this->getAeffect()->numInputs, this->JavaDoubleClass, NULL);
	joutputs = env->NewObjectArray(this->getAeffect()->numOutputs, this->JavaDoubleClass, NULL);
	if (jinputs == NULL) log("** ERROR: out of memory! jinputs");
	if (joutputs == NULL) log("** ERROR: out of memory! joutputs");

	for (int i=0; i<this->getAeffect()->numInputs; i++) {
		double* in = inputs[i];
		jdoubleArray darr = env->NewDoubleArray(sampleFrames);
		
		env->SetDoubleArrayRegion(darr, 0, sampleFrames, in);
        env->SetObjectArrayElement(jinputs, i, darr);
        env->DeleteLocalRef(darr);
	}

	//processDoubleReplacing replaces the output 
	//--> send emtpy output (do not copy output from native to java)
	for (int i=0; i<this->getAeffect()->numOutputs; i++) {
		//create empty float array
		jdoubleArray darr = env->NewDoubleArray(sampleFrames);
		env->SetObjectArrayElement(joutputs, i, darr);
		env->DeleteLocalRef(darr);
	}



	//call java method
	if(this->ProcessDoubleReplacingMethodID == NULL) {
		log("creating new processDoubleRplacing mid");
		this->ProcessDoubleReplacingMethodID = env->GetMethodID(this->JavaPlugClass, "processDoubleReplacing", "([[D[[DI)V");
		if (this->ProcessDoubleReplacingMethodID == NULL) log("** ERROR: cannot find effects .processDoubleReplacing(...)");
	}
	env->CallVoidMethod(this->JavaPlugObj, this->ProcessDoubleReplacingMethodID, jinputs, joutputs, (jint)sampleFrames);



	//jetzt elemente von joutput nach output umkopieren...
	for (int i=0; i<this->getAeffect()->numOutputs; i++) {
		double* out = outputs[i];
		jdoubleArray jout;
		jdouble *jval;

		jout = (jdoubleArray)env->GetObjectArrayElement(joutputs, i);
		jval = env->GetDoubleArrayElements(jout, NULL);
		
		memcpy(out, jval, sampleFrames * sizeof(double));
		
		env->ReleaseDoubleArrayElements(jout, jval, 0);
		env->DeleteLocalRef(jout);
	}
	
	//ARRAYS mit deletelocalref wieder zerstoeren...
	env->DeleteLocalRef(jinputs);
	env->DeleteLocalRef(joutputs);

	::checkException(env);
}
 
