//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// A simple JNI Wrapper 
// for Java as the language to write VSTi Plugins
//
// 2003,2004 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------

	
#ifndef __VSTV20ToPlug__
#include "VSTV20ToPlug.h"
#endif

#ifndef __JNIUtils__
#include "JNIUtils.h"
#endif


#ifdef MACX
#include <pthread.h>
#endif


//Impl of Host -> Plug calls
//Native -> Java 
//-----------------------------------------------------------------------------
VSTV20ToPlug::VSTV20ToPlug (audioMasterCallback audioMaster, int progs, int parms, JavaVM *jvm) 
	: VSTV10ToPlug (audioMaster, progs, parms, jvm) {


		this->isProcessEventsCacheInitialised = false;

		this->ProcessEventsThreadID = 0;
		this->ProcessEventsJEnv = NULL;

		//init cached fields
		this->ProcessEventsMethodID = NULL;
		this->EventConstructorMethodID = NULL;
		this->MidiEventConstructorMethodID = NULL;

		this->VSTEventsClass = NULL;
		this->VSTEventClass = NULL;
		this->VSTMidiEventClass = NULL;

		this->VSTEventsObject = NULL;
	
		this->VSTEventsFieldEvents = NULL;
		this->VSTEventsFieldNumEvents = NULL;
		this->VSTEventsFieldReserved = NULL;

		this->VSTEventFieldType = NULL;
		this->VSTEventFieldByteSize = NULL;
		this->VSTEventFieldDeltaFrames = NULL;
		this->VSTEventFieldFlags = NULL;
		this->VSTEventFieldData = NULL;
		this->VSTMidiEventFieldNoteLength = NULL;
		this->VSTMidiEventFieldNoteOffset = NULL;
		this->VSTMidiEventFieldDetune = NULL;
		this->VSTMidiEventFieldNoteOffVelocity = NULL;
		this->VSTMidiEventFieldReserved1 = NULL;
		this->VSTMidiEventFieldReserved2 = NULL;


		this->isVarIoCacheInitialised = false;
		this->ProcessVarIoMethodID = NULL;

		this->VarIoClass = NULL;
		this->VarIoObject = NULL;

		this->VarIoFieldInputs = NULL;
		this->VarIoFieldOutputs = NULL;
		this->VarIoFieldNumSamplesInput = NULL;
		this->VarIoFieldNumSamplesOutput = NULL;
		this->VarIoFieldNumSamplesInputProcessed = NULL;
		this->VarIoFieldNumSamplesOutputProcessed = NULL;



		this->isSpeakerCacheInitialised = false;
		this->jSpeakerArrClass = NULL;	
		this->jSpeakerPropsClass = NULL; 
		
		this->SpeakerPropsConstructor = NULL;
		this->SetSpeakerArrMethod = NULL;

		this->jSpeakerArrInObject = NULL;
		this->jSpeakerArrOutObject = NULL; 

		this->typeField = NULL; 
		this->numChannelsField = NULL; 
		this->speakersField = NULL; 
			
		this->azimuthField = NULL; 
		this->elevationField = NULL; 
		this->radiusField = NULL; 
		this->reservedField = NULL; 
		this->nameField = NULL; 
		this->propsTypeField = NULL;
		this->futureField = NULL; 

}

//-----------------------------------------------------------------------------------------
VSTV20ToPlug::~VSTV20ToPlug () {

	//Delete global references
	if (this->VSTEventsObject!=NULL) this->JEnv->DeleteGlobalRef(this->VSTEventsObject);
	if (this->VarIoObject!=NULL) this->JEnv->DeleteGlobalRef(this->VarIoObject);

	if (this->jSpeakerArrInObject!=NULL) this->JEnv->DeleteGlobalRef(this->jSpeakerArrInObject);
	if (this->jSpeakerArrOutObject!=NULL) this->JEnv->DeleteGlobalRef(this->jSpeakerArrOutObject); 

}




//-----------------------------------------------------------------------------------------
bool VSTV20ToPlug::getProgramNameIndexed (long category, long index, char* text) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getProgramNameIndexed", "(II)Ljava/lang/String;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getProgramNameIndexed(II)Ljava/lang/String;"); return false;}
	
	jstring ret = (jstring)this->JEnv->CallObjectMethod(this->JavaPlugObj, mid, (jint)category, (jint)index);
	const char* jstr = this->JEnv->GetStringUTFChars(ret, NULL);
	strcpy (text, jstr);

	this->checkException();

	return true;
}

//------------------------------------------------------------------------
bool VSTV20ToPlug::getProductString (char* text) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getProductString", "()Ljava/lang/String;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getProductString()Ljava/lang/String;"); return false;}
	
	jstring ret = (jstring)this->JEnv->CallObjectMethod(this->JavaPlugObj, mid);
	const char* jstr = this->JEnv->GetStringUTFChars(ret, NULL);
	strcpy (text, jstr);

	this->checkException();

	return true;
}

//------------------------------------------------------------------------
bool VSTV20ToPlug::getVendorString (char* text) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getVendorString", "()Ljava/lang/String;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getVendorString()Ljava/lang/String;"); return false;}
	
	jstring ret = (jstring)this->JEnv->CallObjectMethod(this->JavaPlugObj, mid);
	const char* jstr = this->JEnv->GetStringUTFChars(ret, NULL);
	strcpy (text, jstr);

	this->checkException();

	return true;
}

//------------------------------------------------------------------------
VstPlugCategory VSTV20ToPlug::getPlugCategory() { 
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getPlugCategory", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method getPlugCategory()I");
	
	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException();

	switch (ret) {
		case kPlugCategUnknown:
			return kPlugCategUnknown;
			break;
		case kPlugCategEffect:
			return kPlugCategEffect;
			break;
		case kPlugCategSynth:
			return kPlugCategSynth;		
			break;
		case kPlugCategAnalysis:
			return kPlugCategAnalysis;
			break;
		case kPlugCategMastering:
			return kPlugCategMastering;
			break;
		case kPlugCategSpacializer:
			return kPlugCategSpacializer;
			break;
		case kPlugCategRoomFx:
			return kPlugCategRoomFx;
			break;
		case kPlugSurroundFx:	
			return kPlugSurroundFx;
			break;
		case kPlugCategRestoration:
			return kPlugCategRestoration;
			break;
		case kPlugCategOfflineProcess:
			return kPlugCategOfflineProcess;
			break;
		case kPlugCategShell:
			return kPlugCategShell;
			break;
		case kPlugCategGenerator:
			return kPlugCategGenerator;
			break;
		default:
			return kPlugCategUnknown;
			break;
	}
}

long VSTV20ToPlug::canDo (char* text) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "canDo", "(Ljava/lang/String;)I");
	if (mid == NULL) log("** ERROR: cannot find instance-method canDo(Ljava/lang/String;)I");
	
	jstring arg = this->JEnv->NewStringUTF(text);

	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid, arg);
	
	this->checkException();

	return ret;
}

bool VSTV20ToPlug::string2parameter (long index, char *text) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "string2Parameter", "(ILjava/lang/String;)Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method string2Parameter(ILjava/lang/String;)Z"); return false;}

	if (text==NULL) return false;
	jstring arg = this->JEnv->NewStringUTF(text);

	jboolean ret = this->JEnv->CallBooleanMethod(this->JavaPlugObj, mid, (jint)index, arg);
	
	this->checkException();

	return ret!=0;
}

bool VSTV20ToPlug::setBypass (bool onOff) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "setBypass", "(Z)Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method setBypass(Z)Z"); return false;}
	
	jboolean ret = this->JEnv->CallBooleanMethod(this->JavaPlugObj, mid, onOff);
	
	this->checkException();

	return ret!=0;
}


//OPTIONAL


long VSTV20ToPlug::getVendorVersion() {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getVendorVersion", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method getVendorVersion()I");
	
	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException();

	return ret;
}



bool VSTV20ToPlug::getEffectName (char* name)   {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getEffectName", "()Ljava/lang/String;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getEffectName()Ljava/lang/String;"); return false;}
	
	jstring ret = (jstring)this->JEnv->CallObjectMethod(this->JavaPlugObj, mid);
	const char* jstr = this->JEnv->GetStringUTFChars(ret, NULL);
	strcpy (name, jstr);
	
	this->checkException();

	return true;
}

bool VSTV20ToPlug::canParameterBeAutomated(long index) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "canParameterBeAutomated", "(I)Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method canParameterBeAutomated(I)Z"); return false;}
	
	jboolean ret = this->JEnv->CallBooleanMethod(this->JavaPlugObj, mid, (jint)index);
	
	this->checkException();

	return ret!=0;
}

bool VSTV20ToPlug::copyProgram(long destination) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "copyProgram", "(I)Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method copyProgram(I)Z");  return false;}
	
	jboolean ret = this->JEnv->CallBooleanMethod(this->JavaPlugObj, mid, (jint)destination);
	
	this->checkException();

	return ret!=0;
}

long VSTV20ToPlug::fxIdle() {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "fxIdle", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method fxIdle()I");
	
	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException();

	return ret;
}

float VSTV20ToPlug::getChannelParameter(long channel, long index) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getChannelParameter", "(II)F");
	if (mid == NULL) log("** ERROR: cannot find instance-method getChannelParameter(II)F");
	
	jfloat ret = this->JEnv->CallFloatMethod(this->JavaPlugObj, mid, (jint)channel, (jint)index);
	
	this->checkException();

	return ret;
}

long VSTV20ToPlug::getNumCategories() {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getNumCategories", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method getNumCategories()I");
	
	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException();

	return ret;
}

bool VSTV20ToPlug::getInputProperties (long index, VstPinProperties *props) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getInputProperties", "(I)Ljvst/wrapper/valueobjects/VSTPinProperties;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getInputProperties(I)Ljvst/wrapper/valueobjects/VSTPinProperties;"); return false;}
	
	jobject obj = this->JEnv->CallObjectMethod(this->JavaPlugObj, mid, (jint)index);
	if (obj == NULL) {log("** ERROR: getInputProperties() not implemented"); return false;}


	jfieldID fid;
	jstring jstr;
	const char *str;
	jclass cls = this->JEnv->GetObjectClass(obj);

	fid = this->JEnv->GetFieldID(cls, "ArrangementType", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id ArrangementType"); return false;}
	props->arrangementType = this->JEnv->GetIntField(obj, fid);
	
	fid = this->JEnv->GetFieldID(cls, "Flags", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id Flags"); return false;}
	props->flags = this->JEnv->GetIntField(obj, fid);
	
	fid = this->JEnv->GetFieldID(cls, "Label", "Ljava/lang/String;");
	if (fid == NULL) {log("** ERROR: cannot find field-id Label"); return false;}
	jstr = (jstring)this->JEnv->GetObjectField(obj, fid);
	if (jstr!=NULL) {
		str = this->JEnv->GetStringUTFChars(jstr, 0);
		strncpy(props->label,str,63);
	}
	
	fid = this->JEnv->GetFieldID(cls, "ShortLabel", "Ljava/lang/String;");
	if (fid == NULL) {log("** ERROR: cannot find field-id ShortLabel"); return false;}
	jstr = (jstring)this->JEnv->GetObjectField(obj, fid);
	if (jstr!=NULL) {
		str = this->JEnv->GetStringUTFChars(jstr, 0);
		strncpy(props->shortLabel,str,6);
	}

	this->checkException();

	return true;
}

bool VSTV20ToPlug::getOutputProperties (long index, VstPinProperties * props) {	
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getOutputProperties", "(I)Ljvst/wrapper/valueobjects/VSTPinProperties;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getOutputProperties(I)Ljvst/wrapper/valueobjects/VSTPinProperties;"); return false;}
	
	jobject obj = this->JEnv->CallObjectMethod(this->JavaPlugObj, mid, (jint)index);
	
	if (obj==NULL) return false;

	jfieldID fid;
	jstring jstr;
	const char *str;
	jclass cls = this->JEnv->GetObjectClass(obj);

	fid = this->JEnv->GetFieldID(cls, "ArrangementType", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id ArrangementType"); return false;}
	props->arrangementType = this->JEnv->GetIntField(obj, fid);
	
	fid = this->JEnv->GetFieldID(cls, "Flags", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id Flags"); return false;}
	props->flags = this->JEnv->GetIntField(obj, fid);
	
	fid = this->JEnv->GetFieldID(cls, "Label", "Ljava/lang/String;");
	if (fid == NULL) {log("** ERROR: cannot find field-id Label"); return false;}
	jstr = (jstring)this->JEnv->GetObjectField(obj, fid);
	if (jstr!=NULL) {
		str = this->JEnv->GetStringUTFChars(jstr, 0);
		strncpy(props->label,str,63);
	}
	
	fid = this->JEnv->GetFieldID(cls, "ShortLabel", "Ljava/lang/String;");
	if (fid == NULL) {log("** ERROR: cannot find field-id ShortLabel"); return false;}
	jstr = (jstring)this->JEnv->GetObjectField(obj, fid);
	if (jstr!=NULL) {
		str = this->JEnv->GetStringUTFChars(jstr, 0);
		strncpy(props->shortLabel,str,6);
	}

	this->checkException();

	return true;
}

bool VSTV20ToPlug::getErrorText (char* text) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getErrorText", "()Ljava/lang/String;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getErrorText()Ljava/lang/String;"); return false;}
	
	jstring ret = (jstring)this->JEnv->CallObjectMethod(this->JavaPlugObj, mid);
	const char* jstr = this->JEnv->GetStringUTFChars(ret, NULL);
	strcpy (text, jstr);

	this->checkException();

	return true;
}

long VSTV20ToPlug::getGetTailSize () {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getGetTailSize", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method getGetTailSize()I");
	
	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException();

	return ret;
}

bool VSTV20ToPlug::getParameterProperties (long index, VstParameterProperties *p) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getParameterProperties", "(I)Ljvst/wrapper/valueobjects/VSTParameterProperties;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getParameterProperties(I)Ljvst/wrapper/valueobjects/VSTParameterProperties;"); return false;}
	
	jobject obj = this->JEnv->CallObjectMethod(this->JavaPlugObj, mid, (jint)index);
	

	jfieldID fid;
	jstring jstr;
	const char *str;
	jclass cls = this->JEnv->GetObjectClass(obj);

	fid = this->JEnv->GetFieldID(cls, "stepFloat", "F");
	if (fid == NULL) {log("** ERROR: cannot find field-id stepFloat"); return false;}
	p->stepFloat = this->JEnv->GetFloatField(obj, fid);

	fid = this->JEnv->GetFieldID(cls, "smallStepFloat", "F");
	if (fid == NULL) {log("** ERROR: cannot find field-id smallStepFloat"); return false;}
	p->smallStepFloat = this->JEnv->GetFloatField(obj, fid);

	fid = this->JEnv->GetFieldID(cls, "largeStepFloat", "F");
	if (fid == NULL) {log("** ERROR: cannot find field-id largeStepFloat"); return false;}
	p->largeStepFloat = this->JEnv->GetFloatField(obj, fid);

	fid = this->JEnv->GetFieldID(cls, "label", "Ljava/lang/String;");
	if (fid == NULL) {log("** ERROR: cannot find field-id label"); return false;}
	jstr = (jstring)this->JEnv->GetObjectField(obj, fid);
	str = this->JEnv->GetStringUTFChars(jstr, 0);
    strncpy(p->label,str,64);


	fid = this->JEnv->GetFieldID(cls, "flags", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id flags"); return false;}
	p->flags = this->JEnv->GetIntField(obj, fid);

	fid = this->JEnv->GetFieldID(cls, "minInteger", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id minInteger"); return false;}
	p->minInteger = this->JEnv->GetIntField(obj, fid);

	fid = this->JEnv->GetFieldID(cls, "maxInteger", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id maxInteger"); return false;}
	p->maxInteger = this->JEnv->GetIntField(obj, fid);

	fid = this->JEnv->GetFieldID(cls, "stepInteger", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id stepInteger"); return false;}
	p->stepInteger = this->JEnv->GetIntField(obj, fid);

	fid = this->JEnv->GetFieldID(cls, "largeStepInteger", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id largeStepInteger"); return false;}
	p->largeStepInteger = this->JEnv->GetIntField(obj, fid);


	fid = this->JEnv->GetFieldID(cls, "shortLabel", "Ljava/lang/String;");
	if (fid == NULL) {log("** ERROR: cannot find field-id shortLabel"); return false;}
	jstr = (jstring)this->JEnv->GetObjectField(obj, fid);
	str = this->JEnv->GetStringUTFChars(jstr, 0);
    strncpy(p->label,str,6);

	fid = this->JEnv->GetFieldID(cls, "displayIndex", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id displayIndex"); return false;}
	p->displayIndex = (short)this->JEnv->GetIntField(obj, fid);

	fid = this->JEnv->GetFieldID(cls, "category", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id category"); return false;}
	p->category = (short)this->JEnv->GetIntField(obj, fid);

	fid = this->JEnv->GetFieldID(cls, "numParametersInCategory", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id numParametersInCategory"); return false;}
	p->numParametersInCategory = (short)this->JEnv->GetIntField(obj, fid);

	fid = this->JEnv->GetFieldID(cls, "reserved", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id reserved"); return false;}
	p->reserved = (short)this->JEnv->GetIntField(obj, fid);


	fid = this->JEnv->GetFieldID(cls, "categoryLabel", "Ljava/lang/String;");
	if (fid == NULL) {log("** ERROR: cannot find field-id categoryLabel"); return false;}
	jstr = (jstring)this->JEnv->GetObjectField(obj, fid);
	if (jstr!=NULL) {
		str = this->JEnv->GetStringUTFChars(jstr, 0);
		strncpy(p->categoryLabel,str,24);
	}

	this->checkException();

	return true;
}


long VSTV20ToPlug::getVstVersion () {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "getVstVersion", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method getVstVersion()I");
	
	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException();

	return ret;
}

void VSTV20ToPlug::inputConnected (long index, bool state) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "inputConnected", "(IZ)V");
	if (mid == NULL) log("** ERROR: cannot find instance-method inputConnected(IZ)V");
	
	this->JEnv->CallVoidMethod(this->JavaPlugObj, mid, index, state);

	this->checkException();
}

void VSTV20ToPlug::outputConnected (long index, bool state) {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "outputConnected", "(IZ)V");
	if (mid == NULL) log("** ERROR: cannot find instance-method outputConnected(IZ)V");
	
	this->JEnv->CallVoidMethod(this->JavaPlugObj, mid, index, state);

	this->checkException();
}

bool VSTV20ToPlug::keysRequired () {
	this->ensureJavaThreadAttachment();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "keysRequired", "()Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method keysRequired()Z");  return false;}
	
	jboolean ret = this->JEnv->CallBooleanMethod(this->JavaPlugObj, mid);
	
	this->checkException();

	return ret!=0;
}


long VSTV20ToPlug::processEvents (VstEvents* events) {

		
#ifndef MACX
		DWORD threadID;
		threadID = GetCurrentThreadId();
		if (this->ProcessEventsThreadID != threadID) {
			this->ProcessEventsThreadID = threadID;
			
#else
			pthread_t threadID;
			threadID = pthread_self();		
			if (!pthread_equal(threadID,this->ProcessEventsThreadID)){
				this->ProcessEventsThreadID = threadID;
#endif	
		
		

		long stat = this->Jvm->AttachCurrentThread((void**)&this->ProcessEventsJEnv, NULL);
		if (stat<0) log("** ERROR: attaching to .processEvents() THREAD");

		char temp[100];
		sprintf(temp, "ProcessEvents ThreadID=%i", this->ProcessEventsThreadID);
		log(temp);
	}

	if (this->isProcessEventsCacheInitialised==false) this->initProcessEventsCache();

		
	this->ProcessEventsJEnv->SetIntField(this->VSTEventsObject, this->VSTEventsFieldNumEvents, events->numEvents);
	this->ProcessEventsJEnv->SetIntField(this->VSTEventsObject, this->VSTEventsFieldReserved, events->reserved);

	//create events array
	jobjectArray jevents = this->ProcessEventsJEnv->NewObjectArray(events->numEvents, this->VSTEventClass, NULL);
	if (jevents == NULL) {
		log("** ERROR: out of memory! jevents");
		return 0;
	}

	for(int i=0; i<events->numEvents; i++) {
		VstEvent* e = events->events[i];
		jobject jevent;

		if (e->type==kVstMidiType) {
			//Midi Event
			jevent = this->ProcessEventsJEnv->NewObject(this->VSTMidiEventClass, this->MidiEventConstructorMethodID);
			if (jevent == NULL) {
				log("** ERROR: cannot create VSTMidiEvent Object!");
				return 0;
			}
			
			VstMidiEvent* me = (VstMidiEvent*)e;
			this->ProcessEventsJEnv->SetIntField(jevent, this->VSTEventFieldType, me->type);
			this->ProcessEventsJEnv->SetIntField(jevent, this->VSTEventFieldByteSize, me->byteSize);
			this->ProcessEventsJEnv->SetIntField(jevent, this->VSTEventFieldDeltaFrames, me->deltaFrames);
			this->ProcessEventsJEnv->SetIntField(jevent, this->VSTEventFieldFlags, me->flags);
			
			this->ProcessEventsJEnv->SetIntField(jevent, this->VSTMidiEventFieldNoteLength, me->noteLength);
			this->ProcessEventsJEnv->SetIntField(jevent, this->VSTMidiEventFieldNoteOffset, me->noteOffset);
			this->ProcessEventsJEnv->SetByteField(jevent, this->VSTMidiEventFieldDetune, me->detune);
			this->ProcessEventsJEnv->SetByteField(jevent, this->VSTMidiEventFieldNoteOffVelocity, me->noteOffVelocity);
			this->ProcessEventsJEnv->SetByteField(jevent, this->VSTMidiEventFieldReserved1, me->reserved1);
			this->ProcessEventsJEnv->SetByteField(jevent, this->VSTMidiEventFieldReserved2, me->reserved2);
			
			//hack! convert array
			jbyte* b = new jbyte[4];
			for (int k=0; k<4; k++) b[k]=me->midiData[k]; //-127 //???
			
			jbyteArray barr = this->ProcessEventsJEnv->NewByteArray(4);
			this->ProcessEventsJEnv->SetByteArrayRegion(barr, 0, 3, b);
			this->ProcessEventsJEnv->SetObjectField(jevent, this->VSTEventFieldData, barr);	
			this->ProcessEventsJEnv->DeleteLocalRef(barr);	
		}
		else {
			//any other event
			jevent = this->ProcessEventsJEnv->NewObject(this->VSTEventClass, this->EventConstructorMethodID);
			if (jevent == NULL) {
				log("** ERROR: cannot create VSTEvent Object!");
				return 0;
			}

			this->ProcessEventsJEnv->SetIntField(jevent, this->VSTEventFieldType, e->type);
			this->ProcessEventsJEnv->SetIntField(jevent, this->VSTEventFieldByteSize, e->byteSize);
			this->ProcessEventsJEnv->SetIntField(jevent, this->VSTEventFieldDeltaFrames, e->deltaFrames);
			this->ProcessEventsJEnv->SetIntField(jevent, this->VSTEventFieldFlags, e->flags);
			
			//hack! convert array
			jbyte* b = new jbyte[16];
			for (int k=0; i<16; k++) b[k]=e->data[k]; //-127 //???
			
			jbyteArray barr = this->ProcessEventsJEnv->NewByteArray(16);
			this->ProcessEventsJEnv->SetByteArrayRegion(barr, 0, 15, b);
			this->ProcessEventsJEnv->SetObjectField(jevent, this->VSTEventFieldData, barr);	
			this->ProcessEventsJEnv->DeleteLocalRef(barr);
		}

		this->ProcessEventsJEnv->SetObjectArrayElement(jevents, i, jevent);
		this->ProcessEventsJEnv->DeleteLocalRef(jevent); //???
	}

	this->ProcessEventsJEnv->SetObjectField(this->VSTEventsObject, this->VSTEventsFieldEvents, jevents);

	//call java method
	jint ret = this->ProcessEventsJEnv->CallIntMethod(this->JavaPlugObj, this->ProcessEventsMethodID, this->VSTEventsObject);

	this->ProcessEventsJEnv->DeleteLocalRef(jevents);

	::checkException(this->ProcessEventsJEnv);

	return ret;
}


bool VSTV20ToPlug::processVariableIo ( VstVariableIo* varIo) {
	this->ensureJavaThreadAttachment();


	if (this->isVarIoCacheInitialised==false) this->initVarIoCache();


	if(this->JavaFloatClass == NULL) {
		this->JavaFloatClass = this->JEnv->FindClass("[F");
		if (this->JavaFloatClass == NULL) log("** ERROR: cannot find class [F in VarIo");
	}

	jobjectArray  jinputs;
	jobjectArray  joutputs;

	jinputs = this->JEnv->NewObjectArray(this->getAeffect()->numInputs, this->JavaFloatClass, NULL);
	joutputs = this->JEnv->NewObjectArray(this->getAeffect()->numOutputs, this->JavaFloatClass, NULL);
	if (jinputs == NULL) log("** ERROR: out of memory! vario jinputs");
	if (joutputs == NULL) log("** ERROR: out of memory! vario joutputs");

	for (int i=0; i<this->getAeffect()->numInputs; i++) {
		float* in = varIo->inputs[i];
		jfloatArray farr = this->JEnv->NewFloatArray(varIo->numSamplesInput);
		
		this->JEnv->SetFloatArrayRegion(farr, 0, varIo->numSamplesInput, in);
        this->JEnv->SetObjectArrayElement(jinputs, i, farr);
        this->JEnv->DeleteLocalRef(farr);
	}

	for (i=0; i<this->getAeffect()->numOutputs; i++) {
		float* out = varIo->outputs[i];
		jfloatArray farr = this->JEnv->NewFloatArray(varIo->numSamplesOutput);
	
		this->JEnv->SetFloatArrayRegion(farr, 0, varIo->numSamplesOutput, out);
		this->JEnv->SetObjectArrayElement(joutputs, i, farr);
		this->JEnv->DeleteLocalRef(farr);
	}

	//set vario object props
	this->JEnv->SetObjectField(this->VarIoObject, this->VarIoFieldInputs, jinputs);
	this->JEnv->SetObjectField(this->VarIoObject, this->VarIoFieldOutputs, joutputs);
	this->JEnv->SetIntField(this->VarIoObject, this->VarIoFieldNumSamplesInput, varIo->numSamplesInput);
	this->JEnv->SetIntField(this->VarIoObject, this->VarIoFieldNumSamplesInputProcessed, *varIo->numSamplesInputProcessed);
	this->JEnv->SetIntField(this->VarIoObject, this->VarIoFieldNumSamplesOutput, varIo->numSamplesOutput);
	this->JEnv->SetIntField(this->VarIoObject, this->VarIoFieldNumSamplesOutputProcessed, *varIo->numSamplesOutputProcessed);

	jboolean ret = this->JEnv->CallBooleanMethod(this->JavaPlugObj, this->ProcessVarIoMethodID, this->VarIoObject);

	this->JEnv->DeleteLocalRef(jinputs);
	this->JEnv->DeleteLocalRef(joutputs);


	//copy values back to varIo struct
	jinputs = (jobjectArray)this->JEnv->GetObjectField(this->VarIoObject, this->VarIoFieldInputs);
	joutputs = (jobjectArray)this->JEnv->GetObjectField(this->VarIoObject, this->VarIoFieldOutputs);
	for (i=0; i<this->getAeffect()->numInputs; i++) {
		float* in = varIo->inputs[i];
		jfloatArray jin;
		jfloat *jval;

		jin = (jfloatArray)this->JEnv->GetObjectArrayElement(jinputs, i);
		jval = this->JEnv->GetFloatArrayElements(jin, NULL);
		
		memcpy(in, jval, varIo->numSamplesInput * sizeof(float));
		
		this->JEnv->ReleaseFloatArrayElements(jin, jval, 0);
		this->JEnv->DeleteLocalRef(jin);
	}
	for (i=0; i<this->getAeffect()->numOutputs; i++) {
		float* out = varIo->outputs[i];
		jfloatArray jout;
		jfloat *jval;

		jout = (jfloatArray)this->JEnv->GetObjectArrayElement(joutputs, i);
		jval = this->JEnv->GetFloatArrayElements(jout, NULL);
		
		memcpy(out, jval, varIo->numSamplesOutput * sizeof(float));
		
		this->JEnv->ReleaseFloatArrayElements(jout, jval, 0);
		this->JEnv->DeleteLocalRef(jout);
	}
	long x = 0;
	varIo->numSamplesInput = this->JEnv->GetIntField(this->VarIoObject, this->VarIoFieldNumSamplesInput);
	x = this->JEnv->GetIntField(this->VarIoObject, this->VarIoFieldNumSamplesInputProcessed);
	varIo->numSamplesInputProcessed = &x;
	varIo->numSamplesOutput = this->JEnv->GetIntField(this->VarIoObject, this->VarIoFieldNumSamplesOutput);
	x = this->JEnv->GetIntField(this->VarIoObject, this->VarIoFieldNumSamplesOutputProcessed);
	varIo->numSamplesOutputProcessed = &x;
	
	this->JEnv->DeleteLocalRef(jinputs);
	this->JEnv->DeleteLocalRef(joutputs);


	this->checkException();

	return ret!=0;
}

long VSTV20ToPlug::reportCurrentPosition () {
	this->ensureJavaThreadAttachment();

	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "reportCurrentPosition", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method reportCurrentPosition()I");
	
	jint ret = this->JEnv->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException();

	return ret;
}

float* VSTV20ToPlug::reportDestinationBuffer () {
	this->ensureJavaThreadAttachment();

	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "reportDestinationBuffer", "()[F");
	if (mid == NULL) log("** ERROR: cannot find instance-method reportDestinationBuffer()[F");
	
	jfloatArray jret = (jfloatArray)this->JEnv->CallObjectMethod(this->JavaPlugObj, mid);
	jfloat* jval = this->JEnv->GetFloatArrayElements(jret, NULL);
	//do memcopy and free array ressources here?
	/*
	jval = this->JEnv->GetFloatArrayElements(jout, NULL);
	memcpy(out, jval, varIo->numSamplesOutput * sizeof(float));
	this->JEnv->ReleaseFloatArrayElements(jout, jval, 0);
	this->JEnv->DeleteLocalRef(jout);
	*/
	
	this->checkException();

	return jval;
}

void VSTV20ToPlug::setBlockSizeAndSampleRate (long bs, float sr) {
	this->ensureJavaThreadAttachment();

	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugClass, "setBlockSizeAndSampleRate", "(IF)V");
	if (mid == NULL) log("** ERROR: cannot find instance-method setBlockSizeAndSampleRate(IF)V");
	
	this->JEnv->CallVoidMethod(this->JavaPlugObj, mid, bs, sr);

	this->checkException();	
}

bool VSTV20ToPlug::setSpeakerArrangement (VstSpeakerArrangement* pluginInput, VstSpeakerArrangement *pluginOutput) {
	//return true; -->OK!

	this->ensureJavaThreadAttachment();

	if (this->isSpeakerCacheInitialised==false) this->initSpeakerCache();

	
	this->JEnv->SetIntField(this->jSpeakerArrInObject, this->typeField, pluginInput->type);
	this->JEnv->SetIntField(this->jSpeakerArrOutObject, this->typeField, pluginOutput->type);
	this->JEnv->SetIntField(this->jSpeakerArrInObject, this->numChannelsField, pluginInput->numChannels);
	this->JEnv->SetIntField(this->jSpeakerArrOutObject, this->numChannelsField, pluginOutput->numChannels);
	

	if (pluginInput->numChannels > 100) {
		log("* Warning: too many input channels (setSpeakerArrangement)");
		return false;
	}
	jobjectArray jInProps = this->JEnv->NewObjectArray(pluginInput->numChannels, this->jSpeakerPropsClass, NULL);

	for (int i=0; i<pluginInput->numChannels; i++) {
		jobject jSpeakerPropsObject = this->JEnv->NewObject(this->jSpeakerPropsClass, this->SpeakerPropsConstructor);
		if (jSpeakerPropsObject == NULL) {
			log("** ERROR: cannot create VSTSpeakerProperties Object!");
			break;
		}

		this->JEnv->SetFloatField(jSpeakerPropsObject, this->azimuthField, pluginInput->speakers[i].azimuth);
		this->JEnv->SetFloatField(jSpeakerPropsObject, this->elevationField, pluginInput->speakers[i].elevation);
		this->JEnv->SetFloatField(jSpeakerPropsObject, this->radiusField, pluginInput->speakers[i].radius);
		this->JEnv->SetFloatField(jSpeakerPropsObject, this->reservedField, pluginInput->speakers[i].reserved);		
		this->JEnv->SetIntField(jSpeakerPropsObject, this->propsTypeField, pluginInput->speakers[i].type);

		jstring jstr = this->JEnv->NewStringUTF(pluginInput->speakers[i].name);
		this->JEnv->SetObjectField(jSpeakerPropsObject, this->nameField, jstr);
		
		//hack! convert array
		jbyte* c = new jbyte[28];
		for (int j=0; j<28; j++) c[j]=pluginInput->speakers[i].future[j]; // - 127 ???

		jbyteArray farr = this->JEnv->NewByteArray(28);
		//this->JEnv->SetByteArrayRegion(farr, 0, 28, (jbyte*)&pluginInput->speakers[i].future[0]);
		this->JEnv->SetByteArrayRegion(farr, 0, 28, c);
		this->JEnv->SetObjectField(jSpeakerPropsObject, this->futureField, farr);
		this->JEnv->DeleteLocalRef(farr);


		this->JEnv->SetObjectArrayElement(jInProps, i, jSpeakerPropsObject);

		this->JEnv->DeleteLocalRef(jSpeakerPropsObject);
	}


	if (pluginOutput->numChannels > 100) {
		log("* Warning: too many output channels (setSpeakerArrangement)");
		return false;
	}
	jobjectArray jOutProps = this->JEnv->NewObjectArray(pluginOutput->numChannels, this->jSpeakerPropsClass, NULL);

	for (i=0; i<pluginOutput->numChannels; i++) {
		jobject jSpeakerPropsObject = this->JEnv->NewObject(this->jSpeakerPropsClass, this->SpeakerPropsConstructor);
		if (jSpeakerPropsObject == NULL) {
			log("** ERROR: cannot create VSTSpeakerProperties Object!");
			break;
		}

		this->JEnv->SetFloatField(jSpeakerPropsObject, this->azimuthField, pluginOutput->speakers[i].azimuth);
		this->JEnv->SetFloatField(jSpeakerPropsObject, this->elevationField, pluginOutput->speakers[i].elevation);
		this->JEnv->SetFloatField(jSpeakerPropsObject, this->radiusField, pluginOutput->speakers[i].radius);
		this->JEnv->SetFloatField(jSpeakerPropsObject, this->reservedField, pluginOutput->speakers[i].reserved);		
		this->JEnv->SetIntField(jSpeakerPropsObject, this->propsTypeField, pluginOutput->speakers[i].type);

		jstring jstr = this->JEnv->NewStringUTF(pluginOutput->speakers[i].name);
		this->JEnv->SetObjectField(jSpeakerPropsObject, this->nameField, jstr);
		
		//hack! convert array
		jbyte* c = new jbyte[28];
		for (int j=0; j<28; j++) c[j]=pluginOutput->speakers[i].future[j]; // - 127 ???

		jbyteArray farr = this->JEnv->NewByteArray(28);
		//this->JEnv->SetByteArrayRegion(farr, 0, 28, (jbyte*)&pluginOutput->speakers[i].future[0]);
		this->JEnv->SetByteArrayRegion(farr, 0, 28, c);
		this->JEnv->SetObjectField(jSpeakerPropsObject, this->futureField, farr);
		this->JEnv->DeleteLocalRef(farr);


		this->JEnv->SetObjectArrayElement(jOutProps, i, jSpeakerPropsObject);

		this->JEnv->DeleteLocalRef(jSpeakerPropsObject);
	}

	this->JEnv->SetObjectField(this->jSpeakerArrInObject, this->speakersField, jInProps);
	this->JEnv->SetObjectField(this->jSpeakerArrOutObject, this->speakersField, jOutProps);


	jboolean ret = this->JEnv->CallBooleanMethod(this->JavaPlugObj, this->SetSpeakerArrMethod, this->jSpeakerArrInObject, this->jSpeakerArrOutObject);

	this->JEnv->DeleteLocalRef(jInProps);
	this->JEnv->DeleteLocalRef(jOutProps);
	
	this->checkException();

	return ret!=0;
}


//ignored, we have our own UI handling!
//implement that later, so that swing ui can be positioned by native app...
/*
bool VSTV20ToPlug::setViewPosition (long x, long y) {
	return false;
}
*/



//************************************************************************************
void VSTV20ToPlug::initProcessEventsCache () {
	
	//cache fields
	this->ProcessEventsMethodID = this->ProcessEventsJEnv->GetMethodID(this->JavaPlugClass, "processEvents", "(Ljvst/wrapper/valueobjects/VSTEvents;)I");
	if (this->ProcessEventsMethodID == NULL) {
		log("** ERROR: cannot find instance-method processEvents(Ljvst/wrapper/valueobjects/VSTEvents;)I");  
		return;
	}
	this->VSTEventsClass = this->ProcessEventsJEnv->FindClass("jvst/wrapper/valueobjects/VSTEvents");		
	if (this->VSTEventsClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTEvents");
		return;
	}
	this->VSTEventClass = this->ProcessEventsJEnv->FindClass("jvst/wrapper/valueobjects/VSTEvent");		
	if (this->VSTEventClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTEvent");
		return;
	}
	this->VSTMidiEventClass = this->ProcessEventsJEnv->FindClass("jvst/wrapper/valueobjects/VSTMidiEvent");		
	if (this->VSTMidiEventClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTMidiEvent");
		return;
	}



	//Call VSTEvents JAVA Konstruktor
	jmethodID mid = this->ProcessEventsJEnv->GetMethodID(this->VSTEventsClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for VSTEvents!");
		return;
	}
	this->VSTEventsObject = this->ProcessEventsJEnv->NewObject(this->VSTEventsClass, mid);
	if (this->VSTEventsObject == NULL) {
		log("** ERROR: cannot create VST Events Object!");
		return;
	}
	//Create global reference --> use this object accross different threads
	this->VSTEventsObject = this->ProcessEventsJEnv->NewGlobalRef(this->VSTEventsObject);



	this->VSTEventsFieldEvents = this->ProcessEventsJEnv->GetFieldID(this->VSTEventsClass, "events", "[Ljvst/wrapper/valueobjects/VSTEvent;");
	if (this->VSTEventsFieldEvents == NULL) {
		log("** ERROR: cannot find field-id events ([Ljvst/wrapper/valueobjects/VSTEvent;)");
		return;
	}
	this->VSTEventsFieldNumEvents = this->ProcessEventsJEnv->GetFieldID(this->VSTEventsClass, "numEvents", "I");
	if (this->VSTEventsFieldNumEvents == NULL) {
		log("** ERROR: cannot find field-id numEvents");
		return;
	}
	this->VSTEventsFieldReserved = this->ProcessEventsJEnv->GetFieldID(this->VSTEventsClass, "reserved", "I");
	if (this->VSTEventsFieldReserved == NULL) {
		log("** ERROR: cannot find field-id reserved(I) (VSTEvents)");
		return;
	}



	this->EventConstructorMethodID = this->ProcessEventsJEnv->GetMethodID(this->VSTEventClass, "<init>", "()V");
	if (this->EventConstructorMethodID == NULL) {
		log("** ERROR: cannot find default contructor for Event!");
		return;
	}
	this->MidiEventConstructorMethodID = this->ProcessEventsJEnv->GetMethodID(this->VSTMidiEventClass, "<init>", "()V");
	if (this->MidiEventConstructorMethodID == NULL) {
		log("** ERROR: cannot find default contructor for MidiEvent!");
		return;
	}


	this->VSTEventFieldType = this->ProcessEventsJEnv->GetFieldID(this->VSTEventClass, "type", "I");
	if (this->VSTEventFieldType == NULL) {
		log("** ERROR: cannot find field-id type(I) (VSTEvent)");
		return;
	}
	this->VSTEventFieldByteSize = this->ProcessEventsJEnv->GetFieldID(this->VSTEventClass, "byteSize", "I");
	if (this->VSTEventFieldByteSize == NULL) {
		log("** ERROR: cannot find field-id byteSize(I) (VSTEvent)");
		return;
	}
	this->VSTEventFieldDeltaFrames = this->ProcessEventsJEnv->GetFieldID(this->VSTEventClass, "deltaFrames", "I");
	if (this->VSTEventFieldDeltaFrames == NULL) {
		log("** ERROR: cannot find field-id deltaFrames(I) (VSTEvent)");
		return;
	}
	this->VSTEventFieldFlags = this->ProcessEventsJEnv->GetFieldID(this->VSTEventClass, "flags", "I");
	if (this->VSTEventFieldFlags == NULL) {
		log("** ERROR: cannot find field-id flags(I) (VSTEvent)");
		return;
	}
	this->VSTEventFieldData = this->ProcessEventsJEnv->GetFieldID(this->VSTEventClass, "data", "[B");
	if (this->VSTEventFieldData == NULL) {
		log("** ERROR: cannot find field-id data(I) (VSTEvent)");
		return;
	}
	
	

	this->VSTMidiEventFieldNoteLength = this->ProcessEventsJEnv->GetFieldID(this->VSTMidiEventClass, "noteLength", "I");
	if (this->VSTMidiEventFieldNoteLength == NULL) {
		log("** ERROR: cannot find field-id noteLength(I) (VSTMidiEvent)");
		return;
	}
	this->VSTMidiEventFieldNoteOffset = this->ProcessEventsJEnv->GetFieldID(this->VSTMidiEventClass, "noteOffset", "I");
	if (this->VSTMidiEventFieldNoteOffset == NULL) {
		log("** ERROR: cannot find field-id noteOffset(I) (VSTMidiEvent)");
		return;
	}
	this->VSTMidiEventFieldDetune = this->ProcessEventsJEnv->GetFieldID(this->VSTMidiEventClass, "detune", "B");
	if (this->VSTMidiEventFieldDetune == NULL) {
		log("** ERROR: cannot find field-id detune(B) (VSTMidiEvent)");
		return;
	}
	this->VSTMidiEventFieldNoteOffVelocity = this->ProcessEventsJEnv->GetFieldID(this->VSTMidiEventClass, "noteOffVelocity", "B");
	if (this->VSTMidiEventFieldNoteOffVelocity == NULL) {
		log("** ERROR: cannot find field-id noteOffVelocity(B) (VSTMidiEvent)");
		return;
	}
	this->VSTMidiEventFieldReserved1 = this->ProcessEventsJEnv->GetFieldID(this->VSTMidiEventClass, "reserved1", "B");
	if (this->VSTMidiEventFieldReserved1 == NULL) {
		log("** ERROR: cannot find field-id reserved1(B) (VSTMidiEvent)");
		return;
	}
	this->VSTMidiEventFieldReserved2 = this->ProcessEventsJEnv->GetFieldID(this->VSTMidiEventClass, "reserved2", "B");
	if (this->VSTMidiEventFieldReserved2 == NULL) {
		log("** ERROR: cannot find field-id reserved2(B) (VSTMidiEvent)");
		return;
	}


	this->isProcessEventsCacheInitialised = true;

	this->checkException();
}


//************************************************************************************
void VSTV20ToPlug::initVarIoCache () {

	this->VarIoClass = this->JEnv->FindClass("jvst/wrapper/valueobjects/VSTEvent");		
	if (this->VarIoClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTEvent");
		return;
	}


	//Call VarIo JAVA Konstruktor
	jmethodID mid = this->JEnv->GetMethodID(this->VarIoClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for VarIo!");
		return;
	}
	this->VarIoObject = this->JEnv->NewObject(this->VarIoClass, mid);
	if (this->VarIoObject == NULL) {
		log("** ERROR: cannot create VST VarIo Object!");
		return;
	}
	//Create global reference --> use this object accross different threads
	this->VarIoObject = this->JEnv->NewGlobalRef(this->VarIoObject);


	this->ProcessVarIoMethodID = this->JEnv->GetMethodID(this->JavaPlugClass, "processVariableIo", "(Ljvst.wrapper.valueobjects.VSTVariableIO;)Z");
	if (mid == NULL) {
		log("** ERROR: cannot find ProcessVarIo MethodID!");
		return;
	}


	this->VarIoFieldInputs = this->JEnv->GetFieldID(this->VarIoClass, "inputs", "[[F");
	if (this->VarIoFieldInputs == NULL) {
		log("** ERROR: cannot find field-id inputs ([[F");
		return;
	}
	this->VarIoFieldOutputs = this->JEnv->GetFieldID(this->VarIoClass, "outputs", "[[F");
	if (this->VarIoFieldOutputs == NULL) {
		log("** ERROR: cannot find field-id outputs ([[F");
		return;
	}
	this->VarIoFieldNumSamplesInput = this->JEnv->GetFieldID(this->VarIoClass, "numSamplesInput", "I");
	if (this->VarIoFieldNumSamplesInput == NULL) {
		log("** ERROR: cannot find field-id numSamplesInput (I");
		return;
	}
	this->VarIoFieldNumSamplesOutput = this->JEnv->GetFieldID(this->VarIoClass, "numSamplesOutput", "I");
	if (this->VarIoFieldNumSamplesOutput == NULL) {
		log("** ERROR: cannot find field-id numSamplesOutput (I");
		return;
	}
	this->VarIoFieldNumSamplesInputProcessed = this->JEnv->GetFieldID(this->VarIoClass, "numSamplesInputProcessed", "I");
	if (this->VarIoFieldNumSamplesInputProcessed == NULL) {
		log("** ERROR: cannot find field-id numSamplesInputProcessed (I");
		return;
	}
	this->VarIoFieldNumSamplesOutputProcessed = this->JEnv->GetFieldID(this->VarIoClass, "numSamplesOutputProcessed", "I");
	if (this->VarIoFieldNumSamplesOutputProcessed == NULL) {
		log("** ERROR: cannot find field-id numSamplesOutputProcessed (I");
		return;
	}


	this->isVarIoCacheInitialised = true;

	this->checkException();
}


//************************************************************************************
void VSTV20ToPlug::initSpeakerCache() {

	this->jSpeakerArrClass = this->JEnv->FindClass("jvst/wrapper/valueobjects/VSTSpeakerArrangement");
	if (this->jSpeakerArrClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTSpeakerArrangement");
		return;
	}

	//Call JAVA Konstruktor
	jmethodID mid = this->JEnv->GetMethodID(this->jSpeakerArrClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for VSTSpeakerArrangement!");
		return;
	}
	this->jSpeakerArrInObject = this->JEnv->NewObject(this->jSpeakerArrClass, mid);
	if (this->jSpeakerArrInObject == NULL) {
		log("** ERROR: cannot create VSTSpeakerArrangement Object!");
		return;
	}
	this->jSpeakerArrInObject = this->JEnv->NewGlobalRef(this->jSpeakerArrInObject);

	this->jSpeakerArrOutObject = this->JEnv->NewObject(this->jSpeakerArrClass, mid);
	if (this->jSpeakerArrOutObject == NULL) {
		log("** ERROR: cannot create VSTSpeakerArrangement Object!");
		return;
	}
	this->jSpeakerArrOutObject = this->JEnv->NewGlobalRef(this->jSpeakerArrOutObject);


	this->jSpeakerPropsClass = this->JEnv->FindClass("jvst/wrapper/valueobjects/VSTSpeakerProperties");
	if (this->jSpeakerPropsClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTSpeakerProperties");
		return;
	}	
	this->SpeakerPropsConstructor = this->JEnv->GetMethodID(this->jSpeakerPropsClass, "<init>", "()V");
	if (this->SpeakerPropsConstructor == NULL) {
		log("** ERROR: cannot find default contructor for VSTSpeakerProperties!");
		return;
	}

	this->SetSpeakerArrMethod = this->JEnv->GetMethodID(this->JavaPlugClass, "setSpeakerArrangement", "(Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;)Z");
	if (this->SetSpeakerArrMethod == NULL) {
		log("** ERROR: cannot find instance-method setSpeakerArrangement(Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;)Z");
		return;
	}


	 
	this->typeField = this->JEnv->GetFieldID(this->jSpeakerArrClass, "type", "I");
	if (this->typeField == NULL) {
		log("** ERROR: cannot find field-id type (I)");
		return;
	}
	this->numChannelsField = this->JEnv->GetFieldID(this->jSpeakerArrClass, "numChannels", "I");
	if (this->numChannelsField == NULL) {
		log("** ERROR: cannot find field-id numChannels (I)");
		return;
	}
	this->speakersField = this->JEnv->GetFieldID(this->jSpeakerArrClass, "speakers", "[Ljvst/wrapper/valueobjects/VSTSpeakerProperties;");
	if (this->speakersField == NULL) {
		log("** ERROR: cannot find field-id speakers ([Ljvst/wrapper/valueobjects/VSTSpeakerProperties)");
		return;
	}
	
	this->azimuthField = this->JEnv->GetFieldID(this->jSpeakerPropsClass, "azimuth", "F");
	if (this->azimuthField == NULL) {
		log("** ERROR: cannot find field-id azimuth (F)");
		return;
	}
	this->elevationField = this->JEnv->GetFieldID(this->jSpeakerPropsClass, "elevation", "F");
	if (this->elevationField == NULL) {
		log("** ERROR: cannot find field-id elevation (F)");
		return;
	}
	this->radiusField = this->JEnv->GetFieldID(this->jSpeakerPropsClass, "radius", "F");
	if (this->radiusField == NULL) {
		log("** ERROR: cannot find field-id radius (F)");
		return;
	}
	this->reservedField = this->JEnv->GetFieldID(this->jSpeakerPropsClass, "reserved", "F");
	if (this->reservedField == NULL) {
		log("** ERROR: cannot find field-id reserved (F)");
		return;
	}
	this->nameField = this->JEnv->GetFieldID(this->jSpeakerPropsClass, "name", "Ljava/lang/String;");
	if (this->nameField == NULL) {
		log("** ERROR: cannot find field-id name (Ljava/lang/String;)");
		return;
	}
	this->propsTypeField = this->JEnv->GetFieldID(this->jSpeakerPropsClass, "type", "I");
	if (this->typeField == NULL) {
		log("** ERROR: cannot find field-id type (I)");
		return;
	}
	this->futureField = this->JEnv->GetFieldID(this->jSpeakerPropsClass, "future", "[B");
	if (this->futureField == NULL) {
		log("** ERROR: cannot find field-id future ([B)");
		return;
	}

	
	this->isSpeakerCacheInitialised = true;

	this->checkException();
}

