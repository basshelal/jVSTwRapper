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
// A simple JNI Wrapper 
// for Java as the language to write VSTi Plugins
//-
// 2003,2004 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------

#ifndef __VSTV10ToPlug__
#include "VSTV10ToPlug.h"
#endif

#ifndef __JNIUtils__
#include "JNIUtils.h"
#endif

#ifndef __ConfigFileReader__
#include "ConfigFileReader.h"
#endif

#include <stdio.h>
#include <string.h>


extern char DllFileName[50];


//Impl of Host -> Plug calls
//Native -> Java 
//-----------------------------------------------------------------------------
VSTV10ToPlug::VSTV10ToPlug (audioMasterCallback audioMaster, int progs, int parms, JavaVM *jvm)
	: AudioEffectX (audioMaster, progs, parms) {

	log("VSTV10ToPlug KONSTRUCTOR!!!");


	this->Jvm = jvm;
	//this->JavaPlugClass = effectClass;
	
	//init cached fields
	this->JavaPlugObj = NULL;
	this->ProcessMethodID = NULL;
	this->ProcessReplacingMethodID = NULL;
	this->ProcessJEnv = NULL;
	this->JavaFloatClass = NULL;

	this->ProcessReplacingJEnv = NULL;
	this->ProcessReplacingThreadID = 0;

	this->ProcessThreadID = 0;
	this->ThreadID = 0;


	jint res = this->Jvm->GetEnv((void**)&this->JEnv, JNI_VERSION_1_2);
	if (res < 0) {
		log("** ERROR: getting Java env in Wrapper constructor");
	}


	//init cached fields
	GetParameterMethod = NULL;
	SetParameterMethod = NULL;

	chunkdata = NULL;
	chunksize = 0;

	this->ensureJavaThreadAttachment();
 }

//------------------------------------------------------------------------
VSTV10ToPlug::~VSTV10ToPlug () {
	log("Destroying Wrapper");	
	this->ensureJavaThreadAttachment();

	/*
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "destroy", "()V");
	if (mid == NULL) log("** ERROR: cannot find instance-method destroy()V");
	this->JEnv->CallVoidMethod(this->JavaPlugObj, mid);
	*/

	//Delete global reference
	this->JEnv->DeleteGlobalRef(this->JavaPlugObj);

	if(chunkdata!=NULL) { free(chunkdata); chunkdata = NULL; chunksize=0; }

	//hier wird nur gewartet bis der haupt jvm thread der einzigste 
	//java thread ist. die jvm wird aber trotzdem nicht entladen...
	//Jvm->DestroyJavaVM();
}

//------------------------------------------------------------------------
void VSTV10ToPlug::setProgram (VstInt32 program) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "setProgram", "(I)V");
	if (mid == NULL) log("** ERROR: cannot find instance-method setProgram(I)V");
	
	this->JEnv->CallVoidMethod(this->JavaPlugObj, mid, (jint)program);

	this->checkException();
}

VstInt32 VSTV10ToPlug::getProgram () {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getProgram", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method getProgram()I");
	
	return this->JEnv->CallIntMethod(this->JavaPlugObj, mid);

	this->checkException();
}

//------------------------------------------------------------------------
void VSTV10ToPlug::setProgramName (char *name) {
	if (name==NULL) return;

	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "setProgramName", "(Ljava/lang/String;)V");
	if (mid == NULL) log("** ERROR: cannot find instance-method setProgramName(Ljava/lang/String;)V");
	
	jstring arg = this->JEnv->NewStringUTF(name);

	this->JEnv->CallVoidMethod(this->JavaPlugObj, mid, arg);

	this->checkException();
}

//------------------------------------------------------------------------
void VSTV10ToPlug::getProgramName (char *name) {
	if (name==NULL) return;

	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getProgramName", "()Ljava/lang/String;");
	if (mid == NULL) log("** ERROR: cannot find instance-method getProgramName(Ljava/lang/String;)V");
	
	jstring ret = (jstring)this->JEnv->CallObjectMethod(this->JavaPlugObj, mid);
	if (ret==NULL) { strcpy (name, ""); return;}

	const char* jstr = this->JEnv->GetStringUTFChars(ret, NULL);
	strcpy (name, jstr);
	this->JEnv->ReleaseStringUTFChars(ret, jstr);

	this->checkException();
}

//------------------------------------------------------------------------
void VSTV10ToPlug::setParameter (VstInt32 index, float value) {
//Workaround for energyXT: Don't call setParameter from setParameterAutomated
#ifdef WIN32
	if(this->ToHostThread!=GetCurrentThreadId()) {		
#endif
#ifdef MACX
	if(this->ToHostThread!=pthread_self()) {
#endif

	this->ensureJavaThreadAttachment();
	
	if (this->SetParameterMethod==NULL) {
		this->SetParameterMethod = this->JEnv->GetMethodID(this->JavaPlugClass, "setParameter", "(IF)V");
		if (this->SetParameterMethod == NULL) log("** ERROR: cannot find instance-method setParameter(IF)V");
	}
	
	this->JEnv->CallVoidMethod(this->JavaPlugObj, this->SetParameterMethod, (jint)index, (jfloat)value);

	this->checkException();
	}
}

//------------------------------------------------------------------------
float VSTV10ToPlug::getParameter (VstInt32 index)   {
	this->ensureJavaThreadAttachment();

	if (this->GetParameterMethod==NULL) {
		this->GetParameterMethod = this->JEnv->GetMethodID(this->JavaPlugClass, "getParameter", "(I)F");
		if (this->GetParameterMethod == NULL) log("** ERROR: cannot find instance-method getParameter(I)F");
	}

	jfloat ret = this->JEnv->CallFloatMethod(this->JavaPlugObj, this->GetParameterMethod, (jint)index);
	
	this->checkException();
	
	return (float)ret;	
}

//------------------------------------------------------------------------
void VSTV10ToPlug::getParameterName(VstInt32 index, char *label) {
	if (label==NULL) return;

	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getParameterName", "(I)Ljava/lang/String;");
	if (mid == NULL) log("** ERROR: cannot find instance-method getParameterName(I)Ljava/lang/String;");

	jstring ret = (jstring)this->JEnv->CallObjectMethod(this->JavaPlugObj, mid, (jint)index);
	if (ret==NULL) {strcpy (label, ""); return;}

	const char* jstr = this->JEnv->GetStringUTFChars(ret, NULL);
	strcpy (label, jstr);

	this->checkException();
}

//------------------------------------------------------------------------
void VSTV10ToPlug::getParameterDisplay (VstInt32 index, char *text) {
	if (text==NULL) return;

	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getParameterDisplay", "(I)Ljava/lang/String;");
	if (mid == NULL) log("** ERROR: cannot find instance-method getParameterDisplay(I)Ljava/lang/String;");

	jstring ret = (jstring)this->JEnv->CallObjectMethod(this->JavaPlugObj, mid, (jint)index);
	if (ret==NULL) {strcpy (text, ""); return;}

	const char* jstr = this->JEnv->GetStringUTFChars(ret, NULL);
	strcpy (text, jstr);

	this->checkException();
}

//------------------------------------------------------------------------
void VSTV10ToPlug::getParameterLabel (VstInt32 index, char *label) {
	if (label==NULL) return;

	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getParameterLabel", "(I)Ljava/lang/String;");
	if (mid == NULL) log("** ERROR: cannot find instance-method getParameterLabel(I)Ljava/lang/String;");

	jstring ret = (jstring)this->JEnv->CallObjectMethod(this->JavaPlugObj, mid, (jint)index);
	if (ret==NULL) {strcpy (label, ""); return;}

	const char* jstr = this->JEnv->GetStringUTFChars(ret, NULL);
	strcpy (label, jstr);

	this->checkException();
}

//------------------------------------------------------------------------
void VSTV10ToPlug::suspend () {
	this->ensureJavaThreadAttachment();
	
	// Called when Plugin is switched to Off
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "suspend", "()V");
	if (mid == NULL) log("** ERROR: cannot find instance-method suspend()V");
	
	this->JEnv->CallVoidMethod(this->JavaPlugObj, mid);

	this->checkException();
}

//------------------------------------------------------------------------	
void VSTV10ToPlug::resume () {
	this->ensureJavaThreadAttachment();
	
	// Called when Plugin is switched to On
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "resume", "()V");
	if (mid == NULL) log("** ERROR: cannot find instance-method resume()V");
	
	this->JEnv->CallVoidMethod(this->JavaPlugObj, mid);

	this->checkException();

	//call to overwritten method, this is done in all steinberg sample plugs, 
	//so we do it too...
	AudioEffectX::resume();
}	

//------------------------------------------------------------------------
void VSTV10ToPlug::open() {
	this->ensureJavaThreadAttachment();
	
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "open", "()V");
	if (mid == NULL) log("** ERROR: cannot find instance-method open()V");
	
	this->JEnv->CallVoidMethod(this->JavaPlugObj, mid);

	this->checkException();
}

//------------------------------------------------------------------------
void VSTV10ToPlug::close() {
	this->ensureJavaThreadAttachment();
	
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "close", "()V");
	if (mid == NULL) log("** ERROR: cannot find instance-method close()V");
	
	this->JEnv->CallVoidMethod(this->JavaPlugObj, mid);

	this->checkException();
}

//------------------------------------------------------------------------
float VSTV10ToPlug::getVu() {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getVu", "()F");
	if (mid == NULL) log("** ERROR: cannot find instance-method getVu()F");
	
	jfloat ret =  this->JEnv->CallFloatMethod(this->JavaPlugObj, mid);

	this->checkException();

	return ret;
}

//------------------------------------------------------------------------
VstInt32 VSTV10ToPlug::getChunk(void** data, bool isPreset) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getChunk", "([[BZ)I");
	if (mid == NULL) log("** ERROR: cannot find instance-method getChunk([[BZ)I");

	jobjectArray  jdata;//dummy
	jclass byteClass;
	jbyte buf[1] = {-1};//dummy

	byteClass = this->JEnv->FindClass("[B");
	if (byteClass==NULL) log("** ERROR: cannot find Class [B");
	jdata = this->JEnv->NewObjectArray(1, byteClass, NULL);
	jbyteArray barr = this->JEnv->NewByteArray(1);
	this->JEnv->SetByteArrayRegion(barr, 0, 1, buf);
    this->JEnv->SetObjectArrayElement(jdata, 0, barr);
    this->JEnv->DeleteLocalRef(barr);

	jint data_len = this->JEnv->CallIntMethod(this->JavaPlugObj, mid, jdata, (jboolean)isPreset);
	
	//jetzt elemente von jdata[][] nach **data umkopieren...
	//for (int i=0; i<data_len; i++) {
	for (int i=0; i<this->JEnv->GetArrayLength(jdata) ; i++) {
		if(chunksize!=data_len){
			if(chunkdata!=NULL) free(chunkdata); 
			chunkdata=malloc(data_len*sizeof(jbyte));
			chunksize=data_len;
		}
		data[i] = chunkdata;
		jbyte* dat = (jbyte*)data[i];
		jbyteArray jdat;
		jbyte *jval;

		jdat = (jbyteArray)this->JEnv->GetObjectArrayElement(jdata, i);
		jval = this->JEnv->GetByteArrayElements(jdat, NULL);
			
		memcpy(dat, jval, data_len * sizeof(jbyte));
		
		this->JEnv->ReleaseByteArrayElements(jdat, jval, 0);
		this->JEnv->DeleteLocalRef(jdat);
	}
	
	this->checkException();

	return data_len*sizeof(jbyte);
}

//------------------------------------------------------------------------
VstInt32 VSTV10ToPlug::setChunk(void* data, VstInt32 byteSize, bool isPreset) {
	if (data==NULL) return 0;

	this->ensureJavaThreadAttachment();
	jint ret = -1;

	jbyteArray jdata = this->JEnv->NewByteArray(byteSize/sizeof(jbyte));
	this->JEnv->SetByteArrayRegion(jdata, 0, byteSize/sizeof(jbyte), (jbyte*)data);

	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "setChunk", "([BIZ)I");
	if (mid == NULL) log("** ERROR: cannot find instance-method setChunk([BIZ)I");
	ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid, jdata, (jint)(byteSize/sizeof(jbyte)), (jboolean)isPreset);

    this->JEnv->DeleteLocalRef(jdata);

	this->checkException();

	return ret;
}

//------------------------------------------------------------------------
void VSTV10ToPlug::setBlockSize(VstInt32 blockSz) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "setBlockSize", "(I)V");
	if (mid == NULL) log("** ERROR: cannot find instance-method setBlockSize(I)V");
	
	this->JEnv->CallVoidMethod(this->JavaPlugObj, mid, blockSz);

	this->checkException();
}

//------------------------------------------------------------------------
void VSTV10ToPlug::setSampleRate(float sampleRt) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "setSampleRate", "(F)V");
	if (mid == NULL) log("** ERROR: cannot find instance-method setSampleRate(F)V");
	
	this->JEnv->CallVoidMethod(this->JavaPlugObj, mid, sampleRt);

	this->checkException();
}

//------------------------------------------------------------------------
void VSTV10ToPlug::process (float** inputs, float** outputs, VstInt32 sampleFrames) {
	
#ifdef WIN32
		DWORD threadID;
		threadID = GetCurrentThreadId();
		if (this->ProcessThreadID != threadID) {
			this->ProcessThreadID = threadID;			
#endif
#ifdef MACX
		pthread_t threadID;
		threadID = pthread_self();		
		if (!pthread_equal(threadID,this->ProcessThreadID)){
			this->ProcessThreadID = threadID;
#endif	
		
		jint stat = this->Jvm->AttachCurrentThread((void**)&this->ProcessJEnv, NULL);
		if (stat<0) log("** ERROR: attaching to .process() THREAD");

		log("Process ThreadID=%i", this->ProcessThreadID);
	}


	if (this->ProcessJEnv == NULL)this->ProcessJEnv = this->JEnv;	
		
	if(this->JavaFloatClass == NULL) {
		this->JavaFloatClass = this->ProcessJEnv->FindClass("[F");
		if (this->JavaFloatClass == NULL) log("** ERROR: cannot find class [F");
	}

	jobjectArray  jinputs;
	jobjectArray  joutputs;

	jinputs = this->ProcessJEnv->NewObjectArray(this->getAeffect()->numInputs, this->JavaFloatClass, NULL);
	joutputs = this->ProcessJEnv->NewObjectArray(this->getAeffect()->numOutputs, this->JavaFloatClass, NULL);
	if (jinputs == NULL) log("** ERROR: out of memory! jinputs");
	if (joutputs == NULL) log("** ERROR: out of memory! joutputs");

	for (int i=0; i<this->getAeffect()->numInputs; i++) {
		float* in = inputs[i];
		jfloatArray farr = this->ProcessJEnv->NewFloatArray(sampleFrames);
		
		this->ProcessJEnv->SetFloatArrayRegion(farr, 0, sampleFrames, in);
        this->ProcessJEnv->SetObjectArrayElement(jinputs, i, farr);
        this->ProcessJEnv->DeleteLocalRef(farr);
	}

	for (int i=0; i<this->getAeffect()->numOutputs; i++) {
		float* out = outputs[i];
		jfloatArray farr = this->ProcessJEnv->NewFloatArray(sampleFrames);
	
		this->ProcessJEnv->SetFloatArrayRegion(farr, 0, sampleFrames, out);
		this->ProcessJEnv->SetObjectArrayElement(joutputs, i, farr);
		this->ProcessJEnv->DeleteLocalRef(farr);
	}

	//call java method
	if(this->ProcessMethodID == NULL) {
		log("creating new process mid");
		this->ProcessMethodID = this->ProcessJEnv->GetMethodID(this->JavaPlugClass, "process", "([[F[[FI)V");
		if (this->ProcessMethodID == NULL) log("** ERROR: cannot find effects .process(...)");
	}
	this->ProcessJEnv->CallVoidMethod(this->JavaPlugObj, this->ProcessMethodID, jinputs, joutputs, (jint)sampleFrames);

	


	//jetzt elemente von joutput nach output umkopieren...
	for (int i=0; i<this->getAeffect()->numOutputs; i++) {
		float* out = outputs[i];
		jfloatArray jout;
		jfloat *jval;

		jout = (jfloatArray)this->ProcessJEnv->GetObjectArrayElement(joutputs, i);
		jval = this->ProcessJEnv->GetFloatArrayElements(jout, NULL);
		
		memcpy(out, jval, sampleFrames * sizeof(float));
		
		this->ProcessJEnv->ReleaseFloatArrayElements(jout, jval, 0);
		this->ProcessJEnv->DeleteLocalRef(jout);
	}
	
	//ARRAYS mit deletelocalref wieder zerstören...
	this->ProcessJEnv->DeleteLocalRef(jinputs);
	this->ProcessJEnv->DeleteLocalRef(joutputs);

	::checkException(this->ProcessJEnv);
}

//---------------------------------------------------------------------------
void VSTV10ToPlug::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames) {
	
#ifdef WIN32
	DWORD threadID;
	threadID = GetCurrentThreadId();
	if (this->ProcessReplacingThreadID != threadID) {
		this->ProcessReplacingThreadID = threadID;	
#endif
#ifdef MACX
		pthread_t threadID;
		threadID = pthread_self();
		if (!pthread_equal(threadID,this->ProcessReplacingThreadID)){
			this->ProcessReplacingThreadID = threadID;
			
#endif	
	
		jint stat = this->Jvm->AttachCurrentThread((void**)&this->ProcessReplacingJEnv, NULL);
		if (stat<0) log("** ERROR: attaching to .processReplacing() THREAD");

		log("ProcessReplacing ThreadID=%i", this->ProcessReplacingThreadID);
	}
	
	if(this->JavaFloatClass == NULL) {
		this->JavaFloatClass = this->ProcessReplacingJEnv->FindClass("[F");
		if (this->JavaFloatClass == NULL) log("** ERROR: cannot find class [F");
	}

	jobjectArray  jinputs;
	jobjectArray  joutputs;

	jinputs = this->ProcessReplacingJEnv->NewObjectArray(this->getAeffect()->numInputs, this->JavaFloatClass, NULL);
	joutputs = this->ProcessReplacingJEnv->NewObjectArray(this->getAeffect()->numOutputs, this->JavaFloatClass, NULL);
	if (jinputs == NULL) log("** ERROR: out of memory! jinputs");
	if (joutputs == NULL) log("** ERROR: out of memory! joutputs");

	for (int i=0; i<this->getAeffect()->numInputs; i++) {
		float* in = inputs[i];
		jfloatArray farr = this->ProcessReplacingJEnv->NewFloatArray(sampleFrames);
		
		this->ProcessReplacingJEnv->SetFloatArrayRegion(farr, 0, sampleFrames, in);
        this->ProcessReplacingJEnv->SetObjectArrayElement(jinputs, i, farr);
        this->ProcessReplacingJEnv->DeleteLocalRef(farr);
	}

	//processReplacing replaces the output 
	//--> send emtpy output (do not copy output from native to java)
	for (int i=0; i<this->getAeffect()->numOutputs; i++) {
		//create empty float array
		jfloatArray farr = this->ProcessReplacingJEnv->NewFloatArray(sampleFrames);
		this->ProcessReplacingJEnv->SetObjectArrayElement(joutputs, i, farr);
		this->ProcessReplacingJEnv->DeleteLocalRef(farr);
	}



	//call java method
	if(this->ProcessReplacingMethodID == NULL) {
		log("creating new processRplacing mid");
		this->ProcessReplacingMethodID = this->ProcessReplacingJEnv->GetMethodID(this->JavaPlugClass, "processReplacing", "([[F[[FI)V");
		if (this->ProcessReplacingMethodID == NULL) log("** ERROR: cannot find effects .processReplacing(...)");
	}
	this->ProcessReplacingJEnv->CallVoidMethod(this->JavaPlugObj, this->ProcessReplacingMethodID, jinputs, joutputs, (jint)sampleFrames);



	//jetzt elemente von joutput nach output umkopieren...
	for (int i=0; i<this->getAeffect()->numOutputs; i++) {
		float* out = outputs[i];
		jfloatArray jout;
		jfloat *jval;

		jout = (jfloatArray)this->ProcessReplacingJEnv->GetObjectArrayElement(joutputs, i);
		jval = this->ProcessReplacingJEnv->GetFloatArrayElements(jout, NULL);
		
		memcpy(out, jval, sampleFrames * sizeof(float));
		
		this->ProcessReplacingJEnv->ReleaseFloatArrayElements(jout, jval, 0);
		this->ProcessReplacingJEnv->DeleteLocalRef(jout);
	}
	
	//ARRAYS mit deletelocalref wieder zerstören...
	this->ProcessReplacingJEnv->DeleteLocalRef(jinputs);
	this->ProcessReplacingJEnv->DeleteLocalRef(joutputs);

	::checkException(this->ProcessReplacingJEnv);
}










//---------------------------------------------------------------------------
int VSTV10ToPlug::initJavaSide(jclass effectClass) {
	this->ensureJavaThreadAttachment();


	if(effectClass==NULL) return -1;
	this->JavaPlugClass = effectClass;

    jlong wri=((jlong)((long)this));
	//JAVA Konstruktor
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "<init>", "(J)V");
	if (mid == NULL) {
		log("** ERROR: cannot find contructor <init>(long)");
		this->checkException(); //print stack trace!
		return -1;
	}

	if (this->checkException()) return -1;

	this->JavaPlugObj = this->JEnv->NewObject(this->JavaPlugClass, mid,wri);
	if (this->JavaPlugObj == NULL) {	
		log("** ERROR: cannot instantiate Java Plugin Object, \nException occured in constructor?\nPlease look at the generated LOG files.");
		this->checkException(); //print stack trace!
		return -1;
	}

	if (this->checkException()) return -1;

	//Create global reference --> use this object accross different threads
	this->JavaPlugObj = this->JEnv->NewGlobalRef(this->JavaPlugObj);

	if (this->checkException()) return -1;



	//setting correct numPrograms value
	mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getNumPrograms", "()I");
	if (mid == NULL) {
		log("** ERROR: cannot find effects .getNumPrograms(I)");
		this->checkException(); //print stack trace!
		return -1;
	}
	jint num = this->JEnv->CallIntMethod(this->JavaPlugObj, mid);
	this->getAeffect()->numPrograms = num;
	this->numPrograms = num;

	if (this->checkException()) return -1;


	//setting correct numParams value
	mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getNumParams", "()I");
	if (mid == NULL) {
		log("** ERROR: cannot find effects .getNumParams(I)");
		this->checkException(); //print stack trace!
		return -1;
	}
	num = this->JEnv->CallIntMethod(this->JavaPlugObj, mid);
	this->getAeffect()->numParams = num;
	this->numParams = num;	


	log("Plugin initJavaSide ok!");
	if (this->checkException()) return -1;
	else return 0;
}

//-----------------------------------------------------------------------------
void VSTV10ToPlug::ensureJavaThreadAttachment() {
	
#ifdef WIN32
	DWORD threadID;
	threadID = GetCurrentThreadId();
	if (this->ThreadID != threadID) {
		this->ThreadID = threadID;
#endif
#ifdef MACX
	pthread_t threadID;
	threadID = pthread_self();
	if (!pthread_equal(threadID,this->ThreadID)){
		this->ThreadID = threadID;
#endif
		jint stat = this->Jvm->AttachCurrentThread((void**)&this->JEnv, NULL);
		if (stat<0) log("** ERROR: attaching to native THREAD in VSTV10ToPlug Wrapper!");

		log("New wrapper ThreadID=%i", this->ThreadID);
	}
	
	//ultra important, if a pending exception isnt cleared, all following calls will fail...
	this->checkException();
}

void VSTV10ToPlug::setNumParams(VstInt32 num){
	this->getAeffect()->numParams=num;
	this->numParams=num;
}


bool VSTV10ToPlug::checkException() {
	return ::checkException(this->JEnv);	
}
