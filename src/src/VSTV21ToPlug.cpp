//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// A JNI Wrapper 
// for Java as the language to write VSTi Plugins
//-------------------------------------------------------------------------------------------------------

	
#ifndef __VSTV21ToPlug__
#include "VSTV21ToPlug.h"
#endif

#ifndef __JNIUtils__
#include "JNIUtils.h"
#endif




//Impl of Host -> Plug calls
//Native -> Java 
//-----------------------------------------------------------------------------
VSTV21ToPlug::VSTV21ToPlug (audioMasterCallback audioMaster, int progs, int parms, JavaVM *jvm) 
	: VSTV20ToPlug (audioMaster, progs, parms, jvm) {

}
	
//-----------------------------------------------------------------------------------------
VSTV21ToPlug::~VSTV21ToPlug () {

}

//-----------------------------------------------------------------------------------------
long VSTV21ToPlug::getMidiProgramName (long channel, MidiProgramName* midiProgramName) { 
	this->ensureJavaThreadAttachment();

	jclass jMidiProgramClass = this->JEnv->FindClass("jvst/wrapper/valueobjects/MidiProgramName");
	if (jMidiProgramClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.MidiProgramName");
		return 0;
	}

	jmethodID mid = this->JEnv->GetMethodID(jMidiProgramClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for MidiProgramName!");
		return 0;
	}
	jobject jMidiProgramObject = this->JEnv->NewObject(jMidiProgramClass, mid);
	if (jMidiProgramObject == NULL) {
		log("** ERROR: cannot create MidiProgramName Object!");
		return 0;
	}

	jfieldID thisProgramIndexField = this->JEnv->GetFieldID(jMidiProgramClass, "thisProgramIndex", "I");
	if (thisProgramIndexField == NULL) {
		log("** ERROR: cannot find field-id thisProgramIndex (I)");
		return 0;
	}
	jfieldID parentCategoryIndexField = this->JEnv->GetFieldID(jMidiProgramClass, "parentCategoryIndex", "I");
	if (parentCategoryIndexField == NULL) {
		log("** ERROR: cannot find field-id parentCategoryIndex (I)");
		return 0;
	}
	jfieldID flagsField = this->JEnv->GetFieldID(jMidiProgramClass, "flags", "I");
	if (flagsField == NULL) {
		log("** ERROR: cannot find field-id flags (I)");
		return 0;
	}
	jfieldID nameField = this->JEnv->GetFieldID(jMidiProgramClass, "name", "Ljava/lang/String;");
	if (nameField == NULL) {
		log("** ERROR: cannot find field-id name (Ljava/lang/String;)");
		return 0;
	}
	jfieldID midiProgramField = this->JEnv->GetFieldID(jMidiProgramClass, "midiProgram", "B");
	if (midiProgramField == NULL) {
		log("** ERROR: cannot find field-id midiProgram (B)");
		return 0;
	}
	jfieldID midiBankMsbField = this->JEnv->GetFieldID(jMidiProgramClass, "midiBankMsb", "B");
	if (midiBankMsbField == NULL) {
		log("** ERROR: cannot find field-id midiBankMsb (B)");
		return 0;
	}
	jfieldID midiBankLsbField = this->JEnv->GetFieldID(jMidiProgramClass, "midiBankLsb", "B");
	if (midiBankLsbField == NULL) {
		log("** ERROR: cannot find field-id midiBankLsb (B)");
		return 0;
	}
	jfieldID reservedField = this->JEnv->GetFieldID(jMidiProgramClass, "reserved", "B");
	if (reservedField == NULL) {
		log("** ERROR: cannot find field-id reserved (B)");
		return 0;
	}
	
	//in parameter!
	this->JEnv->SetIntField(jMidiProgramObject, thisProgramIndexField, midiProgramName->thisProgramIndex);
	
	mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getMidiProgramName", "(ILjvst/wrapper/valueobjects/MidiProgramName;)I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getMidiProgramName(ILjvst/wrapper/valueobjects/MidiProgramName;)I"); return 0;}
	
	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid, channel, jMidiProgramObject);
	
	//read out parameters
	midiProgramName->flags = this->JEnv->GetIntField(jMidiProgramObject, flagsField);
	midiProgramName->parentCategoryIndex = this->JEnv->GetIntField(jMidiProgramObject, parentCategoryIndexField);
	
	jstring jstr = (jstring)this->JEnv->GetObjectField(jMidiProgramObject, nameField);
	const char* str = this->JEnv->GetStringUTFChars(jstr, NULL);
	strncpy (midiProgramName->name, str, 63);
	this->JEnv->ReleaseStringUTFChars(jstr, str);
	this->JEnv->DeleteLocalRef(jstr);

	midiProgramName->midiBankLsb = this->JEnv->GetByteField(jMidiProgramObject, midiBankLsbField);
	midiProgramName->midiBankMsb = this->JEnv->GetByteField(jMidiProgramObject, midiBankMsbField);
	midiProgramName->midiProgram = this->JEnv->GetByteField(jMidiProgramObject, midiProgramField);
	midiProgramName->reserved = this->JEnv->GetByteField(jMidiProgramObject, reservedField);

	this->checkException();

	return ret;
}

//-----------------------------------------------------------------------------------------
long VSTV21ToPlug::getCurrentMidiProgram (long channel, MidiProgramName* midiProgramName) { 
	this->ensureJavaThreadAttachment();

	jclass jMidiProgramClass = this->JEnv->FindClass("jvst/wrapper/valueobjects/MidiProgramName");
	if (jMidiProgramClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.MidiProgramName");
		return 0;
	}

	jmethodID mid = this->JEnv->GetMethodID(jMidiProgramClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for MidiProgramName!");
		return 0;
	}
	jobject jMidiProgramObject = this->JEnv->NewObject(jMidiProgramClass, mid);
	if (jMidiProgramObject == NULL) {
		log("** ERROR: cannot create MidiProgramName Object!");
		return 0;
	}

	jfieldID thisProgramIndexField = this->JEnv->GetFieldID(jMidiProgramClass, "thisProgramIndex", "I");
	if (thisProgramIndexField == NULL) {
		log("** ERROR: cannot find field-id thisProgramIndex (I)");
		return 0;
	}
	jfieldID parentCategoryIndexField = this->JEnv->GetFieldID(jMidiProgramClass, "parentCategoryIndex", "I");
	if (parentCategoryIndexField == NULL) {
		log("** ERROR: cannot find field-id parentCategoryIndex (I)");
		return 0;
	}
	jfieldID flagsField = this->JEnv->GetFieldID(jMidiProgramClass, "flags", "I");
	if (flagsField == NULL) {
		log("** ERROR: cannot find field-id flags (I)");
		return 0;
	}
	jfieldID nameField = this->JEnv->GetFieldID(jMidiProgramClass, "name", "Ljava/lang/String;");
	if (nameField == NULL) {
		log("** ERROR: cannot find field-id name (Ljava/lang/String;)");
		return 0;
	}
	jfieldID midiProgramField = this->JEnv->GetFieldID(jMidiProgramClass, "midiProgram", "B");
	if (midiProgramField == NULL) {
		log("** ERROR: cannot find field-id flags (B)");
		return 0;
	}
	jfieldID midiBankMsbField = this->JEnv->GetFieldID(jMidiProgramClass, "midiBankMsb", "B");
	if (midiBankMsbField == NULL) {
		log("** ERROR: cannot find field-id midiBankMsb (B)");
		return 0;
	}
	jfieldID midiBankLsbField = this->JEnv->GetFieldID(jMidiProgramClass, "midiBankLsb", "B");
	if (midiBankLsbField == NULL) {
		log("** ERROR: cannot find field-id midiBankLsb (B)");
		return 0;
	}
	jfieldID reservedField = this->JEnv->GetFieldID(jMidiProgramClass, "reserved", "B");
	if (reservedField == NULL) {
		log("** ERROR: cannot find field-id reserved (B)");
		return 0;
	}

	mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getCurrentMidiProgram", "(ILjvst/wrapper/valueobjects/MidiProgramName;)I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getCurrentMidiProgram(ILjvst/wrapper/valueobjects/MidiProgramName;)I"); return 0;}
	
	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid, channel, jMidiProgramObject);
	
	//read out parameters
	midiProgramName->thisProgramIndex = this->JEnv->GetIntField(jMidiProgramObject, thisProgramIndexField);
	midiProgramName->flags = this->JEnv->GetIntField(jMidiProgramObject, flagsField);
	midiProgramName->parentCategoryIndex = this->JEnv->GetIntField(jMidiProgramObject, parentCategoryIndexField);
	
	jstring jstr = (jstring)this->JEnv->GetObjectField(jMidiProgramObject, nameField);
	const char* str = this->JEnv->GetStringUTFChars(jstr, NULL);
	strncpy (midiProgramName->name, str, 63);
	this->JEnv->ReleaseStringUTFChars(jstr, str);
	this->JEnv->DeleteLocalRef(jstr);

	midiProgramName->midiBankLsb = this->JEnv->GetByteField(jMidiProgramObject, midiBankLsbField);
	midiProgramName->midiBankMsb = this->JEnv->GetByteField(jMidiProgramObject, midiBankMsbField);
	midiProgramName->midiProgram = this->JEnv->GetByteField(jMidiProgramObject, midiProgramField);
	midiProgramName->reserved = this->JEnv->GetByteField(jMidiProgramObject, reservedField);

	this->checkException();

	return ret;
}

//-----------------------------------------------------------------------------------------
long VSTV21ToPlug::getMidiProgramCategory (long channel, MidiProgramCategory* category) { 
	this->ensureJavaThreadAttachment();

	jclass jMidiProgramCategoryClass = this->JEnv->FindClass("jvst/wrapper/valueobjects/MidiProgramCategory");
	if (jMidiProgramCategoryClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.MidiProgramCategory");
		return 0;
	}

	jmethodID mid = this->JEnv->GetMethodID(jMidiProgramCategoryClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for MidiProgramCategory!");
		return 0;
	}
	jobject jMidiProgramCategoryObject = this->JEnv->NewObject(jMidiProgramCategoryClass, mid);
	if (jMidiProgramCategoryObject == NULL) {
		log("** ERROR: cannot create MidiProgramCategory Object!");
		return 0;
	}

	jfieldID thisCategoryIndexField = this->JEnv->GetFieldID(jMidiProgramCategoryClass, "thisCategoryIndex", "I");
	if (thisCategoryIndexField == NULL) {
		log("** ERROR: cannot find field-id thisCategoryIndex (I)");
		return 0;
	}
	jfieldID parentCategoryIndexField = this->JEnv->GetFieldID(jMidiProgramCategoryClass, "parentCategoryIndex", "I");
	if (parentCategoryIndexField == NULL) {
		log("** ERROR: cannot find field-id parentCategoryIndex (I)");
		return 0;
	}
	jfieldID flagsField = this->JEnv->GetFieldID(jMidiProgramCategoryClass, "flags", "I");
	if (flagsField == NULL) {
		log("** ERROR: cannot find field-id flags (I)");
		return 0;
	}
	jfieldID nameField = this->JEnv->GetFieldID(jMidiProgramCategoryClass, "name", "Ljava/lang/String;");
	if (nameField == NULL) {
		log("** ERROR: cannot find field-id name (Ljava/lang/String;)");
		return 0;
	}



	//in parameter!
	this->JEnv->SetIntField(jMidiProgramCategoryObject, thisCategoryIndexField, category->thisCategoryIndex);
	
	mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getMidiProgramCategory", "(ILjvst/wrapper/valueobjects/MidiProgramCategory;)I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getMidiProgramCategory(ILjvst/wrapper/valueobjects/MidiProgramCategory;)I"); return 0;}
	
	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid, channel, jMidiProgramCategoryObject);
	
	//read out parameters
	category->flags = this->JEnv->GetIntField(jMidiProgramCategoryObject, flagsField);
	category->parentCategoryIndex = this->JEnv->GetIntField(jMidiProgramCategoryObject, parentCategoryIndexField);
	
	jstring jstr = (jstring)this->JEnv->GetObjectField(jMidiProgramCategoryObject, nameField);
	const char* str = this->JEnv->GetStringUTFChars(jstr, NULL);
	strncpy (category->name, str, 63);
	this->JEnv->ReleaseStringUTFChars(jstr, str);
	this->JEnv->DeleteLocalRef(jstr);

	this->checkException();

	return ret;
}

//-----------------------------------------------------------------------------------------
bool VSTV21ToPlug::hasMidiProgramsChanged (long channel) { 
	this->ensureJavaThreadAttachment();

	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "hasMidiProgramsChanged", "(I)Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method hasMidiProgramsChanged(I)Z"); return false;}
	
	jboolean ret = this->JEnv->CallBooleanMethod(this->JavaPlugObj, mid, channel);
	
	this->checkException();

	return ret!=0;
}

//-----------------------------------------------------------------------------------------
bool VSTV21ToPlug::getMidiKeyName (long channel, MidiKeyName* keyName) { 
	this->ensureJavaThreadAttachment();

	jclass jMidiKeyNameClass = this->JEnv->FindClass("jvst/wrapper/valueobjects/MidiKeyName");
	if (jMidiKeyNameClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.MidiKeyName");
		return false;
	}

	jmethodID mid = this->JEnv->GetMethodID(jMidiKeyNameClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for MidiKeyName!");
		return false;
	}
	jobject jMidikeyNameObject = this->JEnv->NewObject(jMidiKeyNameClass, mid);
	if (jMidikeyNameObject == NULL) {
		log("** ERROR: cannot create MidiKeyName Object!");
		return false;
	}

	jfieldID thisProgramIndexField = this->JEnv->GetFieldID(jMidiKeyNameClass, "thisProgramIndex", "I");
	if (thisProgramIndexField == NULL) {
		log("** ERROR: cannot find field-id thisProgramIndex (I)");
		return false;
	}
	jfieldID thisKeyNumberField = this->JEnv->GetFieldID(jMidiKeyNameClass, "thisKeyNumber", "I");
	if (thisKeyNumberField == NULL) {
		log("** ERROR: cannot find field-id thisKeyNumber (I)");
		return false;
	}
	jfieldID flagsField = this->JEnv->GetFieldID(jMidiKeyNameClass, "flags", "I");
	if (flagsField == NULL) {
		log("** ERROR: cannot find field-id flags (I)");
		return false;
	}
	jfieldID reservedField = this->JEnv->GetFieldID(jMidiKeyNameClass, "reserved", "I");
	if (reservedField == NULL) {
		log("** ERROR: cannot find field-id reserved (I)");
		return false;
	}
	jfieldID keyNameField = this->JEnv->GetFieldID(jMidiKeyNameClass, "keyName", "Ljava/lang/String;");
	if (keyNameField == NULL) {
		log("** ERROR: cannot find field-id keyName (Ljava/lang/String;)");
		return false;
	}



	//in parameter!
	this->JEnv->SetIntField(jMidikeyNameObject, thisKeyNumberField, keyName->thisKeyNumber);
	this->JEnv->SetIntField(jMidikeyNameObject, thisProgramIndexField, keyName->thisProgramIndex);
	
	mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getMidiKeyName", "(ILjvst/wrapper/valueobjects/MidiKeyName;)Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getMidiKeyName(ILjvst/wrapper/valueobjects/MidiKeyName;)Z"); return false;}
	
	jboolean ret = this->JEnv->CallBooleanMethod(this->JavaPlugObj, mid, channel, jMidikeyNameObject);
	
	//read out parameters
	jstring jstr = (jstring)this->JEnv->GetObjectField(jMidikeyNameObject, keyNameField);
	const char* str = this->JEnv->GetStringUTFChars(jstr, NULL);
	strncpy (keyName->keyName, str, 63);
	keyName->keyName[64] = 0;
	this->JEnv->ReleaseStringUTFChars(jstr, str);
	this->JEnv->DeleteLocalRef(jstr);
	
	this->checkException();

	return ret!=0;
}

//-----------------------------------------------------------------------------------------
bool VSTV21ToPlug::beginSetProgram () { 
	this->ensureJavaThreadAttachment();

	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "beginSetProgram", "()Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method beginSetProgram()Z"); return false;}
	
	jboolean ret = this->JEnv->CallBooleanMethod(this->JavaPlugObj, mid);
	
	this->checkException();

	return ret!=0;
} 

//-----------------------------------------------------------------------------------------
bool VSTV21ToPlug::endSetProgram () { 
	this->ensureJavaThreadAttachment();

	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "endSetProgram", "()Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method endSetProgram()Z"); return false;}
	
	jboolean ret = this->JEnv->CallBooleanMethod(this->JavaPlugObj, mid);
	
	this->checkException();

	return ret!=0;
}   


