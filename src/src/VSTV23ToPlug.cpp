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
VstInt32 VSTV23ToPlug::setTotalSampleToProcess (VstInt32 value) {	
	this->ensureJavaThreadAttachment();

	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "setTotalSampleToProcess", "(I)I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method setTotalSampleToProcess(I)I"); return value;}
	
	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid, value);
	
	this->checkException();

	return ret;
} 

//-----------------------------------------------------------------------------------------
VstInt32 VSTV23ToPlug::getNextShellPlugin (char* name) { 
	if (name==NULL) return 0;

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
VstInt32 VSTV23ToPlug::startProcess () { 
	this->ensureJavaThreadAttachment();

	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "startProcess", "()I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method startProcess()I"); return 0;}

	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException();

	return ret;
}	

//-----------------------------------------------------------------------------------------
VstInt32 VSTV23ToPlug::stopProcess () { 
	this->ensureJavaThreadAttachment();

	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "stopProcess", "()I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method stopProcess()I"); return 0;}

	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException();

	return ret;	
}	

