//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// A JNI Wrapper 
// for Java as the language to write VSTi Plugins
//-------------------------------------------------------------------------------------------------------

	
#ifndef __VSTV23ToPlug__
#include "VSTV23ToPlug.h"
#endif

#ifndef __JNIUtils__
#include "JNIUtils.h"
#endif




//Impl of Host -> Plug calls
//Native -> Java 
//-----------------------------------------------------------------------------
VSTV23ToPlug::VSTV23ToPlug (audioMasterCallback audioMaster, int progs, int parms, JavaVM *jvm) 
	: VSTV22ToPlug (audioMaster, progs, parms, jvm) {

}

//-----------------------------------------------------------------------------------------
VSTV23ToPlug::~VSTV23ToPlug () {

}

//-----------------------------------------------------------------------------------------
long VSTV23ToPlug::setTotalSampleToProcess (long value) {	
	this->ensureJavaThreadAttachment();

	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "setTotalSampleToProcess", "(I)I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method setTotalSampleToProcess(I)I"); return value;}
	
	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid, value);
	
	this->checkException();

	return ret;
} 

//-----------------------------------------------------------------------------------------
long VSTV23ToPlug::getNextShellPlugin (char* name) { 
	this->ensureJavaThreadAttachment();

	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getNextShellPlugin", "(Ljava/lang/String;)I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getNextShellPlugin(Ljava/lang/String;)I"); return -1;}

	jstring str = this->JEnv->NewStringUTF("dummy - replace me with REAL name!");
	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid, str);
	
	if (str!=NULL) {
		const char* jstr = this->JEnv->GetStringUTFChars(str, NULL);
		strncpy (name, jstr, 63);
		this->JEnv->ReleaseStringUTFChars(str, jstr);
		this->JEnv->DeleteLocalRef(str);
	} else ret = 0L;

	
	this->checkException();

	return ret;
}

//-----------------------------------------------------------------------------------------
long VSTV23ToPlug::startProcess () { 
	this->ensureJavaThreadAttachment();

	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "startProcess", "()I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method startProcess()I"); return 0;}

	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException();

	return ret;
}	

//-----------------------------------------------------------------------------------------
long VSTV23ToPlug::stopProcess () { 
	this->ensureJavaThreadAttachment();

	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "stopProcess", "()I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method stopProcess()I"); return 0;}

	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException();

	return ret;	
}	

