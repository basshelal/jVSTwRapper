 //-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Java -> Native calls impls
//-
// 2003,2004 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------



#ifndef _Included_jvst_wrapper_communication_VSTV20ToHost
#include "VSTV20ToHost.h"
#endif

#ifndef __JNIUtils__
#include "JNIUtils.h"
#endif

#ifndef __VSTV24ToPlug__
#include "VSTV24ToPlug.h"
#endif


extern VSTV24ToPlug* getWrapperInstance(JNIEnv *env, jobject obj);



//Impl of Plug -> Host calls
//Java -> Native
//impls for VSTV20ToHost.java native calls
//**********************************************



//cached fieds
//******************************

//TimeInfo
void InitTimeInfoCache(JNIEnv* env);
bool IsTimeInfoCacheInitialised = false;
jclass TimeInfoClass = NULL;
jobject TimeInfoObject = NULL;

jfieldID TimeInfoFieldSamplePos = NULL;     
jfieldID TimeInfoFieldSampleRate = NULL;
jfieldID TimeInfoFieldNanoSeconds = NULL;		
jfieldID TimeInfoFieldPPQPos = NULL;		
jfieldID TimeInfoFieldTempo = NULL;		
jfieldID TimeInfoFieldBarStartPos = NULL;		
jfieldID TimeInfoFieldCycleStartPos = NULL;	
jfieldID TimeInfoFieldCycleEndPos = NULL;	
jfieldID TimeInfoFieldTimeSigNumerator = NULL;
jfieldID TimeInfoFieldTimeSigDenominator = NULL;
jfieldID TimeInfoFieldSmpteOffset = NULL;
jfieldID TimeInfoFieldSmpteFrameRate = NULL;	
jfieldID TimeInfoFieldSamplesToNextClock = NULL;
jfieldID TimeInfoFieldFlags = NULL;		


//Events
void InitEventsCache(JNIEnv* env);
bool IsEventsCacheInitialised = false;

jclass VSTEventsClass = NULL;
jclass VSTEventClass = NULL;
jclass MidiEventClass = NULL;

jfieldID EventsFieldEvents = NULL;
jfieldID EventsFieldNumEvents = NULL;
jfieldID EventsFieldReserved = NULL;

jfieldID EventFieldType = NULL;
jfieldID EventFieldByteSize = NULL;
jfieldID EventFieldDeltaFrames = NULL;
jfieldID EventFieldFlags = NULL;
jfieldID EventFieldData = NULL;
jfieldID MidiEventFieldNoteLength = NULL;
jfieldID MidiEventFieldNoteOffset = NULL;
jfieldID MidiEventFieldDetune = NULL;
jfieldID MidiEventFieldNoteOffVelocity = NULL;
jfieldID MidiEventFieldReserved1 = NULL;
jfieldID MidiEventFieldReserved2 = NULL;


//Speakers
void initSpeakerCache(JNIEnv* env);
bool isSpeakerCacheInitialised = false;

jclass jSpeakerArrClass = NULL;		
jclass jSpeakerPropsClass = NULL;

jmethodID SpeakerPropsConstructor = NULL;

jobject jSpeakerArrInObject = NULL;
jobject jSpeakerArrOutObject = NULL;

jfieldID typeField = NULL;
jfieldID numChannelsField = NULL;
jfieldID speakersField = NULL;
	
jfieldID azimuthField = NULL;
jfieldID elevationField = NULL;
jfieldID radiusField = NULL;
jfieldID reservedField = NULL;
jfieldID nameField = NULL;
jfieldID propsTypeField = NULL;
jfieldID futureField = NULL;


#define VST_EVENTS_MAX 256

struct BigVstEvents
{
    long numEvents;
    long reserved;
    VstEvent* events[VST_EVENTS_MAX];
};

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    canHostDo
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_canHostDo
		(JNIEnv * env, jobject obj, jstring str) {
	log("canHostDo");

	char t[255];
	jint ret = -1L;
	
	if (str==NULL) return ret;

	const char* text = env->GetStringUTFChars(str, NULL);
	strcpy(t, text);
	env->ReleaseStringUTFChars(str, text);
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) ret = WrapperInstance->canHostDo(t);

	return ret;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    sizeWindow
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_sizeWindow
(JNIEnv * env, jobject obj, jint width, jint height){
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->sizeWindow(width,height);
	else return false;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    ioChanged
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_ioChanged
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->ioChanged();
	else return false;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getTimeInfo
 * Signature: (I)Ljvst/wrapper/valueobjects/VSTTimeInfo;
 */
JNIEXPORT jobject JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getTimeInfo
		(JNIEnv* env, jobject obj, jint filt) {
	
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) {
		if (IsTimeInfoCacheInitialised==false) InitTimeInfoCache(env);

		VstTimeInfo* ti = WrapperInstance->getTimeInfo(filt);

		env->SetDoubleField(TimeInfoObject, TimeInfoFieldSamplePos, ti->samplePos);
		env->SetDoubleField(TimeInfoObject, TimeInfoFieldSampleRate, ti->sampleRate);
		env->SetDoubleField(TimeInfoObject, TimeInfoFieldNanoSeconds, ti->nanoSeconds);
		env->SetDoubleField(TimeInfoObject, TimeInfoFieldPPQPos, ti->ppqPos);
		env->SetDoubleField(TimeInfoObject, TimeInfoFieldTempo, ti->tempo);
		env->SetDoubleField(TimeInfoObject, TimeInfoFieldBarStartPos, ti->barStartPos);
		env->SetDoubleField(TimeInfoObject, TimeInfoFieldCycleStartPos, ti->cycleStartPos);
		env->SetDoubleField(TimeInfoObject, TimeInfoFieldCycleEndPos, ti->cycleEndPos);
		
		env->SetIntField(TimeInfoObject, TimeInfoFieldTimeSigNumerator, ti->timeSigNumerator);
		env->SetIntField(TimeInfoObject, TimeInfoFieldTimeSigDenominator, ti->timeSigDenominator);
		env->SetIntField(TimeInfoObject, TimeInfoFieldSmpteFrameRate, ti->smpteFrameRate);
		env->SetIntField(TimeInfoObject, TimeInfoFieldSmpteOffset, ti->smpteOffset);
		env->SetIntField(TimeInfoObject, TimeInfoFieldSamplesToNextClock, ti->samplesToNextClock);
		env->SetIntField(TimeInfoObject, TimeInfoFieldFlags, ti->flags);

        checkAndThrowException(env);

		return TimeInfoObject;
	}
	else return NULL;
}



/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    sendVstEventsToHost
 * Signature: (Ljvst/wrapper/valueobjects/VSTEvents;)Z
 */

JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_sendVstEventsToHost
	(JNIEnv* env, jobject obj, jobject events) {
	 
     VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
		if (WrapperInstance!=NULL) {
			
            BigVstEvents vstEventsToHost;
            VstMidiEvent vstMidiEventsToHost[VST_EVENTS_MAX];  
            VstEvent vstEventToHost[VST_EVENTS_MAX];        

			if (IsEventsCacheInitialised==false) InitEventsCache(env);


			VstEvents* evts = (VstEvents*)&vstEventsToHost;
			evts->numEvents = env->GetIntField(events, EventsFieldNumEvents);
			evts->reserved = env->GetIntField(events, EventsFieldReserved);

			jobjectArray jevents = (jobjectArray)env->GetObjectField(events, EventsFieldEvents);
			for (int i=0; i < env->GetArrayLength(jevents); i++) {
				
				//CAUTION: I only VST_EVENTS_MAX events will be transmited to Host

				if (i>=VST_EVENTS_MAX) break; 

				VstEvent* event;
				jobject jevent = env->GetObjectArrayElement(jevents, i);

				jint typ = env->GetIntField(jevent, EventFieldType);

				if (typ==kVstMidiType) {					
					VstMidiEvent* mevent = &(vstMidiEventsToHost[i]);
					
					mevent->type = typ;
					mevent->byteSize = env->GetIntField(jevent, EventFieldByteSize);
					mevent->deltaFrames = env->GetIntField(jevent, EventFieldDeltaFrames);
					mevent->flags = env->GetIntField(jevent, EventFieldFlags);

					jbyteArray jdata = (jbyteArray)env->GetObjectField(jevent, EventFieldData);
					jbyte* elmts = env->GetByteArrayElements(jdata, NULL);
					for (int j=0; j<env->GetArrayLength(jdata); j++) {
						if (j>=3) break; // 3 Bytes of midi data
						mevent->midiData[j] = elmts[j];
					}
					mevent->midiData[3] = 0; //reserved
					env->ReleaseByteArrayElements(jdata, elmts, 0);
					env->DeleteLocalRef(jdata);
					
					mevent->noteLength = env->GetIntField(jevent, MidiEventFieldNoteLength);
					mevent->noteOffset = env->GetIntField(jevent, MidiEventFieldNoteOffset);

					mevent->detune = env->GetByteField(jevent, MidiEventFieldDetune);
					mevent->noteOffVelocity = env->GetByteField(jevent, MidiEventFieldNoteOffVelocity);
					mevent->reserved1 = env->GetByteField(jevent, MidiEventFieldReserved1);
					mevent->reserved2 = env->GetByteField(jevent, MidiEventFieldReserved2);

					event = (VstEvent*)mevent;
				}
				else {
					event = &(vstEventToHost[i]);

					event->type = typ;
					event->byteSize = env->GetIntField(jevent, EventFieldByteSize);
					event->deltaFrames = env->GetIntField(jevent, EventFieldDeltaFrames);
					event->flags = env->GetIntField(jevent, EventFieldFlags);

					jbyteArray jdata = (jbyteArray)env->GetObjectField(jevent, EventFieldData);
					jbyte* elmts = env->GetByteArrayElements(jdata, NULL);
					for (int j=0; j<env->GetArrayLength(jdata); j++) {
						if (j>=16) break; //16 Bytes Data
						event->data[j] = elmts[j];
					}
					env->ReleaseByteArrayElements(jdata, elmts, 0);
					env->DeleteLocalRef(jdata);
				}

				evts->events[i] = event;
			}

			checkAndThrowException(env);			
			return WrapperInstance->sendVstEventsToHost(evts);
		}
		else return 0;
 }




/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    needIdle
 * Signature: ()Z
 */

//dm: deprecated as of vst2.4
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_needIdle
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->needIdle();
	else return false;
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    updateSampleRate
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_updateSampleRate
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->updateSampleRate();
	else return 0.00;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    updateBlockSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_updateBlockSize
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->updateBlockSize();
	else return 0L;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getAutomationState
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getAutomationState
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getAutomationState();
	else return 0L;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getCurrentProcessLevel
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getCurrentProcessLevel
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getCurrentProcessLevel();
	else return 0L;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getDirectory
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getDirectory
		(JNIEnv* env, jobject obj) {

	jstring ret = NULL;

	//TODO:
	//Mac hosts return a FSSpec* here!!! (instead of char*)
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) ret = env->NewStringUTF((char *)WrapperInstance->getDirectory());

	return ret;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getHostProductString
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getHostProductString
		(JNIEnv* env, jobject obj) {
	
	jstring ret = NULL;
	char text[255];
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->getHostProductString(text);
	if (WrapperInstance!=NULL) ret = env->NewStringUTF(text);

	return ret;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getHostVendorVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getHostVendorVersion
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getHostVendorVersion();
	else return -1L;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getHostVendorString
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getHostVendorString
		(JNIEnv* env, jobject obj) {

	jstring ret = NULL;
	char text[255];
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->getHostVendorString(text);
	if (WrapperInstance!=NULL) ret = env->NewStringUTF(text);

	return ret;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getHostLanguage
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getHostLanguage
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getHostLanguage();
	else return 0L;
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getInputLatency
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getInputLatency
		(JNIEnv* env, jobject obj) {
   VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getInputLatency();
	else return 0L;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getOutputLatency
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getOutputLatency
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getOutputLatency();
	else return 0L;
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getNumAutomatableParameters
 * Signature: ()I
 */

//dm: deprecated as of vst2.4
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getNumAutomatableParameters
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getNumAutomatableParameters();
	else return 0L;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getParameterQuantization
 * Signature: ()I
 */

//dm: deprecated as of vst2.4
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getParameterQuantization
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getParameterQuantization();
	else return 0L;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getSpeakerArrangement
 * Signature: (Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getSpeakerArrangement
		(JNIEnv* env, jobject obj, jobject jinput, jobject joutput) {

    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) {
		if (isSpeakerCacheInitialised==false) initSpeakerCache(env);


		VstSpeakerArrangement* input;
		VstSpeakerArrangement* output;

		jboolean ret = WrapperInstance->getSpeakerArrangement(&input, &output);


		env->SetIntField(jinput, typeField, input->type);
		env->SetIntField(joutput, typeField, output->type);
		env->SetIntField(jinput, numChannelsField, input->numChannels);
		env->SetIntField(joutput, numChannelsField, output->numChannels);
		

		jobjectArray jInProps = env->NewObjectArray(input->numChannels, jSpeakerPropsClass, NULL);

		for (int i=0; i<input->numChannels; i++) {
			jobject jSpeakerPropsObject = env->NewObject(jSpeakerPropsClass, SpeakerPropsConstructor);
			if (jSpeakerPropsObject == NULL) {
				log("** ERROR: cannot create VSTSpeakerProperties Object!");
				break;
			}

			env->SetFloatField(jSpeakerPropsObject, azimuthField, input->speakers[i].azimuth);
			env->SetFloatField(jSpeakerPropsObject, elevationField, input->speakers[i].elevation);
			env->SetFloatField(jSpeakerPropsObject, radiusField, input->speakers[i].radius);
			env->SetFloatField(jSpeakerPropsObject, reservedField, input->speakers[i].reserved);		
			env->SetIntField(jSpeakerPropsObject, propsTypeField, input->speakers[i].type);

			jstring jstr = env->NewStringUTF(input->speakers[i].name);
			env->SetObjectField(jSpeakerPropsObject, nameField, jstr);
			
			//hack! convert array
			jbyte* c = new jbyte[28];
			for (int j=0; j<28; j++) c[j]=input->speakers[i].future[j]; // - 127 ???
			
			jbyteArray farr = env->NewByteArray(28);
			env->SetByteArrayRegion(farr, 0, 28, c);
			env->SetObjectField(jSpeakerPropsObject, futureField, farr);
			env->DeleteLocalRef(farr);


			env->SetObjectArrayElement(jInProps, i, jSpeakerPropsObject);

			env->DeleteLocalRef(jSpeakerPropsObject);
			delete [] c;
		}


		jobjectArray jOutProps = env->NewObjectArray(output->numChannels, jSpeakerPropsClass, NULL);

		for (i=0; i<output->numChannels; i++) {
			jobject jSpeakerPropsObject = env->NewObject(jSpeakerPropsClass, SpeakerPropsConstructor);
			if (jSpeakerPropsObject == NULL) {
				log("** ERROR: cannot create VSTSpeakerProperties Object!");
				break;
			}

			env->SetFloatField(jSpeakerPropsObject, azimuthField, output->speakers[i].azimuth);
			env->SetFloatField(jSpeakerPropsObject, elevationField, output->speakers[i].elevation);
			env->SetFloatField(jSpeakerPropsObject, radiusField, output->speakers[i].radius);
			env->SetFloatField(jSpeakerPropsObject, reservedField, output->speakers[i].reserved);		
			env->SetIntField(jSpeakerPropsObject, propsTypeField, output->speakers[i].type);

			jstring jstr = env->NewStringUTF(output->speakers[i].name);
			env->SetObjectField(jSpeakerPropsObject, nameField, jstr);
			
			//hack! convert array
			jbyte* c = new jbyte[28];
			for (int j=0; j<28; j++) c[j]=output->speakers[i].future[j]; // - 127 ???
			
			jbyteArray farr = env->NewByteArray(28);
			env->SetByteArrayRegion(farr, 0, 28, c);
			env->SetObjectField(jSpeakerPropsObject, futureField, farr);
			env->DeleteLocalRef(farr);


			env->SetObjectArrayElement(jOutProps, i, jSpeakerPropsObject);

			env->DeleteLocalRef(jSpeakerPropsObject);
			delete[] c;
		}

		env->SetObjectField(jinput, speakersField, jInProps);
		env->SetObjectField(joutput, speakersField, jOutProps);


		env->DeleteLocalRef(jInProps);
		env->DeleteLocalRef(jOutProps);
		
		checkAndThrowException(env);

		return ret;
	}
	else return 0;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    hasExternalBuffer
 * Signature: (Z)V
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_hasExternalBuffer
		(JNIEnv* env, jobject obj, jboolean state) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->hasExternalBuffer(state != 0 ? true : false);
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    isSynth
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_isSynth
		(JNIEnv* env, jobject obj, jboolean state) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->isSynth(state != 0 ? true : false);
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    noTail
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_noTail
		(JNIEnv* env, jobject obj, jboolean state) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->noTail(state != 0 ? true : false);
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    setOutputSamplerate
 * Signature: (F)V
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_setOutputSamplerate
		(JNIEnv* env, jobject obj, jfloat sr) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->setOutputSamplerate(sr);
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    tempoAt
 * Signature: (I)I
 */

//dm: deprecated as of vst2.4
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_tempoAt
		(JNIEnv* env, jobject obj, jint pos) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->tempoAt(pos);
	else return 0L;
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    updateDisplay
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_updateDisplay
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->updateDisplay();
	else return false;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    wantAsyncOperation
 * Signature: (Z)V
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_wantAsyncOperation
		(JNIEnv* env, jobject obj, jboolean state) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->wantAsyncOperation(state != 0 ? true : false);
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    wantEvents
 * Signature: (I)V
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_wantEvents
		(JNIEnv* env, jobject obj, jint fil) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->wantEvents(fil);
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    willProcessReplacing
 * Signature: ()I
 */

//dm: deprecated as of vst2.4
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_willProcessReplacing
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->willProcessReplacing();
	else return 0L;
}








void InitTimeInfoCache(JNIEnv* env) {
	TimeInfoClass = env->FindClass("jvst/wrapper/valueobjects/VSTTimeInfo");		
	if (TimeInfoClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTTimeInfo");
		return;
	}


	//Call JAVA Konstruktor
	jmethodID mid = env->GetMethodID(TimeInfoClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for TimeInfo!");
		return;
	}
	TimeInfoObject = env->NewObject(TimeInfoClass, mid);
	if (TimeInfoObject == NULL) {
		log("** ERROR: cannot create VST TimeInfo Object!");
		return;
	}
	//create global reference!
	TimeInfoObject = env->NewGlobalRef(TimeInfoObject);


	TimeInfoFieldSamplePos = env->GetFieldID(TimeInfoClass, "samplePos", "D");
	if (TimeInfoFieldSamplePos == NULL) {
		log("** ERROR: cannot find field-id samplePos (D)");
		return;
	}
	TimeInfoFieldSampleRate = env->GetFieldID(TimeInfoClass, "sampleRate", "D");
	if (TimeInfoFieldSampleRate == NULL) {
		log("** ERROR: cannot find field-id sampleRate (D)");
		return;
	}
	TimeInfoFieldNanoSeconds = env->GetFieldID(TimeInfoClass, "nanoSeconds", "D");
	if (TimeInfoFieldNanoSeconds == NULL) {
		log("** ERROR: cannot find field-id nanoSeconds (D)");
		return;
	}
	TimeInfoFieldPPQPos = env->GetFieldID(TimeInfoClass, "ppqPos", "D");
	if (TimeInfoFieldPPQPos == NULL) {
		log("** ERROR: cannot find field-id ppqPos (D)");
		return;
	}
	TimeInfoFieldTempo = env->GetFieldID(TimeInfoClass, "tempo", "D");
	if (TimeInfoFieldTempo == NULL) {
		log("** ERROR: cannot find field-id tempo (D)");
		return;
	}
	TimeInfoFieldBarStartPos = env->GetFieldID(TimeInfoClass, "barStartPos", "D");
	if (TimeInfoFieldBarStartPos == NULL) {
		log("** ERROR: cannot find field-id barStartPos (D)");
		return;
	}
	TimeInfoFieldCycleStartPos = env->GetFieldID(TimeInfoClass, "cycleStartPos", "D");
	if (TimeInfoFieldCycleStartPos == NULL) {
		log("** ERROR: cannot find field-id cycleStartPos (D)");
		return;
	}
	TimeInfoFieldCycleEndPos = env->GetFieldID(TimeInfoClass, "cycleEndPos", "D");
	if (TimeInfoFieldCycleEndPos == NULL) {
		log("** ERROR: cannot find field-id cycleEndPos (D)");
		return;
	}

	TimeInfoFieldTimeSigNumerator = env->GetFieldID(TimeInfoClass, "timeSigNumerator", "I");
	if (TimeInfoFieldTimeSigNumerator == NULL) {
		log("** ERROR: cannot find field-id timeSigNumerator (I)");
		return;
	}
	TimeInfoFieldTimeSigDenominator = env->GetFieldID(TimeInfoClass, "timeSigDenominator", "I");
	if (TimeInfoFieldTimeSigDenominator == NULL) {
		log("** ERROR: cannot find field-id timeSigDenominator (I)");
		return;
	}
	TimeInfoFieldSmpteOffset = env->GetFieldID(TimeInfoClass, "smpteOffset", "I");
	if (TimeInfoFieldSmpteOffset == NULL) {
		log("** ERROR: cannot find field-id smpteOffset (I)");
		return;
	}
	TimeInfoFieldSmpteFrameRate = env->GetFieldID(TimeInfoClass, "smpteFrameRate", "I");
	if (TimeInfoFieldSmpteFrameRate == NULL) {
		log("** ERROR: cannot find field-id smpteFrameRate (I)");
		return;
	}
	TimeInfoFieldSamplesToNextClock = env->GetFieldID(TimeInfoClass, "samplesToNextClock", "I");
	if (TimeInfoFieldSamplesToNextClock == NULL) {
		log("** ERROR: cannot find field-id samplesToNextClock (I)");
		return;
	}
	TimeInfoFieldFlags = env->GetFieldID(TimeInfoClass, "flags", "I");
	if (TimeInfoFieldFlags == NULL) {
		log("** ERROR: cannot find field-id flags (I)");
		return;
	}

	IsTimeInfoCacheInitialised = true;
}


void InitEventsCache(JNIEnv* env) {
	VSTEventsClass = env->FindClass("jvst/wrapper/valueobjects/VSTEvents");		
	if (VSTEventsClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTEvents");
		return;
	}
	VSTEventClass = env->FindClass("jvst/wrapper/valueobjects/VSTEvent");		
	if (VSTEventClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTEvent");
		return;
	}
	MidiEventClass = env->FindClass("jvst/wrapper/valueobjects/VSTMidiEvent");		
	if (MidiEventClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTMidiEvent");
		return;
	}


	EventsFieldEvents = env->GetFieldID(VSTEventsClass, "events", "[Ljvst/wrapper/valueobjects/VSTEvent;");
	if (EventsFieldEvents == NULL) {
		log("** ERROR: cannot find field-id events ([Ljvst/wrapper/valueobjects/VSTEvent;)");
		return;
	}
	EventsFieldNumEvents = env->GetFieldID(VSTEventsClass, "numEvents", "I");
	if (EventsFieldNumEvents == NULL) {
		log("** ERROR: cannot find field-id numEvents");
		return;
	}
	EventsFieldReserved = env->GetFieldID(VSTEventsClass, "reserved", "I");
	if (EventsFieldReserved == NULL) {
		log("** ERROR: cannot find field-id reserved(I) (Events)");
		return;
	}


	EventFieldType = env->GetFieldID(VSTEventClass, "type", "I");
	if (EventFieldType == NULL) {
		log("** ERROR: cannot find field-id type(I) (Event)");
		return;
	}
	EventFieldByteSize = env->GetFieldID(VSTEventClass, "byteSize", "I");
	if (EventFieldByteSize == NULL) {
		log("** ERROR: cannot find field-id byteSize(I) (Event)");
		return;
	}
	EventFieldDeltaFrames = env->GetFieldID(VSTEventClass, "deltaFrames", "I");
	if (EventFieldDeltaFrames == NULL) {
		log("** ERROR: cannot find field-id deltaFrames(I) (Event)");
		return;
	}
	EventFieldFlags = env->GetFieldID(VSTEventClass, "flags", "I");
	if (EventFieldFlags == NULL) {
		log("** ERROR: cannot find field-id flags(I) (Event)");
		return;
	}
	EventFieldData = env->GetFieldID(VSTEventClass, "data", "[B");
	if (EventFieldData == NULL) {
		log("** ERROR: cannot find field-id data(I) (Event)");
		return;
	}
	MidiEventFieldNoteLength = env->GetFieldID(MidiEventClass, "noteLength", "I");
	if (MidiEventFieldNoteLength == NULL) {
		log("** ERROR: cannot find field-id noteLength(I) (MidiEvent)");
		return;
	}
	MidiEventFieldNoteOffset = env->GetFieldID(MidiEventClass, "noteOffset", "I");
	if (MidiEventFieldNoteOffset == NULL) {
		log("** ERROR: cannot find field-id noteOffset(I) (MidiEvent)");
		return;
	}
	MidiEventFieldDetune = env->GetFieldID(MidiEventClass, "detune", "B");
	if (MidiEventFieldDetune == NULL) {
		log("** ERROR: cannot find field-id detune(B) (MidiEvent)");
		return;
	}
	MidiEventFieldNoteOffVelocity = env->GetFieldID(MidiEventClass, "noteOffVelocity", "B");
	if (MidiEventFieldNoteOffVelocity == NULL) {
		log("** ERROR: cannot find field-id noteOffVelocity(B) (MidiEvent)");
		return;
	}
	MidiEventFieldReserved1 = env->GetFieldID(MidiEventClass, "reserved1", "B");
	if (MidiEventFieldReserved1 == NULL) {
		log("** ERROR: cannot find field-id reserved1(B) (MidiEvent)");
		return;
	}
	MidiEventFieldReserved2 = env->GetFieldID(MidiEventClass, "reserved2", "B");
	if (MidiEventFieldReserved2 == NULL) {
		log("** ERROR: cannot find field-id reserved2(B) (MidiEvent)");
		return;
	}

	IsEventsCacheInitialised = true;
}



void initSpeakerCache(JNIEnv* env) {
	
	jSpeakerArrClass = env->FindClass("jvst/wrapper/valueobjects/VSTSpeakerArrangement");
	if (jSpeakerArrClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTSpeakerArrangement");
		return;
	}
		
	//Call JAVA Konstruktor
	jmethodID mid = env->GetMethodID(jSpeakerArrClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for VSTSpeakerArrangement!");
		return;
	}
	jSpeakerArrInObject = env->NewObject(jSpeakerArrClass, mid);
	if (jSpeakerArrInObject == NULL) {
		log("** ERROR: cannot create VSTSpeakerArrangement Object!");
		return;
	}
	jSpeakerArrInObject = env->NewGlobalRef(jSpeakerArrInObject);

	jSpeakerArrOutObject = env->NewObject(jSpeakerArrClass, mid);
	if (jSpeakerArrOutObject == NULL) {
		log("** ERROR: cannot create VSTSpeakerArrangement Object!");
		return;
	}
	jSpeakerArrOutObject = env->NewGlobalRef(jSpeakerArrOutObject);


	jSpeakerPropsClass = env->FindClass("jvst/wrapper/valueobjects/VSTSpeakerProperties");
	if (jSpeakerPropsClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTSpeakerProperties");
		return;
	}	
	SpeakerPropsConstructor = env->GetMethodID(jSpeakerPropsClass, "<init>", "()V");
	if (SpeakerPropsConstructor == NULL) {
		log("** ERROR: cannot find default contructor for VSTSpeakerProperties!");
		return;
	}



	typeField = env->GetFieldID(jSpeakerArrClass, "type", "I");
	if (typeField == NULL) {
		log("** ERROR: cannot find field-id type (I)");
		return;
	}
	numChannelsField = env->GetFieldID(jSpeakerArrClass, "numChannels", "I");
	if (numChannelsField == NULL) {
		log("** ERROR: cannot find field-id numChannels (I)");
		return;
	}
	speakersField = env->GetFieldID(jSpeakerArrClass, "speakers", "[Ljvst/wrapper/valueobjects/VSTSpeakerProperties");
	if (speakersField == NULL) {
		log("** ERROR: cannot find field-id speakersField ([Ljvst/wrapper/valueobjects/VSTSpeakerProperties)");
		return;
	}
	
	azimuthField = env->GetFieldID(jSpeakerPropsClass, "azimuth", "F");
	if (azimuthField == NULL) {
		log("** ERROR: cannot find field-id azimuth (F)");
		return;
	}
	elevationField = env->GetFieldID(jSpeakerPropsClass, "elevation", "F");
	if (elevationField == NULL) {
		log("** ERROR: cannot find field-id elevation (F)");
		return;
	}
	radiusField = env->GetFieldID(jSpeakerPropsClass, "radius", "F");
	if (radiusField == NULL) {
		log("** ERROR: cannot find field-id radius (F)");
		return;
	}
	reservedField = env->GetFieldID(jSpeakerPropsClass, "reserved", "F");
	if (reservedField == NULL) {
		log("** ERROR: cannot find field-id reserved (F)");
		return;
	}
	nameField = env->GetFieldID(jSpeakerPropsClass, "name", "Ljava/lang/String;");
	if (nameField == NULL) {
		log("** ERROR: cannot find field-id name (Ljava/lang/String;)");
		return;
	}
	propsTypeField = env->GetFieldID(jSpeakerPropsClass, "type", "I");
	if (typeField == NULL) {
		log("** ERROR: cannot find field-id type (I)");
		return;
	}
	futureField = env->GetFieldID(jSpeakerPropsClass, "future", "[B");
	if (futureField == NULL) {
		log("** ERROR: cannot find field-id future ([B)");
		return;
	}
	
	isSpeakerCacheInitialised = true;
}
