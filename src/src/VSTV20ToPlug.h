//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Definitions for the Java VSTi Wrapper class
//-
// 2003,2004 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------

#ifndef __VSTV20ToPlug__
#define __VSTV20ToPlug__


#ifndef __VSTV10ToPlug__
#include "VSTV10ToPlug.h"
#endif




//------------------------------------------------------------------------
class VSTV20ToPlug : public VSTV10ToPlug {

	public:
		VSTV20ToPlug (audioMasterCallback audioMaster, int prg, int prm, JavaVM *jvm);
		virtual ~VSTV20ToPlug ();


		//HOST -> PLUG

		//vst 2.0 REQUIRED
		//*****************
		virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
		virtual bool getProductString (char* text);
		virtual bool getVendorString (char* text);
		virtual VstPlugCategory getPlugCategory ();
		virtual VstInt32 canDo (char* text);
		virtual bool string2parameter (VstInt32 index, char* text);
		virtual bool setBypass (bool onOff);
		


		//vst 2.0 OPTIONAL
		//*****************
		virtual bool getEffectName (char* name);
		virtual VstInt32 getVendorVersion ();

		virtual bool canParameterBeAutomated (VstInt32 index);
		virtual bool copyProgram(VstInt32 destination);
		virtual VstInt32 fxIdle();
		virtual float getChannelParameter(VstInt32 channel, VstInt32 index);
		virtual VstInt32 getNumCategories();

		virtual bool getInputProperties (VstInt32 index, VstPinProperties *properties);
		virtual bool getOutputProperties (VstInt32 index, VstPinProperties * properties);
		virtual bool getErrorText (char* text);
		virtual VstInt32 getGetTailSize ();
		virtual bool getParameterProperties (VstInt32 index, VstParameterProperties* p);

		virtual VstInt32 getVstVersion ();
		virtual void inputConnected (VstInt32 index, bool state);
		virtual void outputConnected (VstInt32 index, bool state);
		virtual bool keysRequired ();

		virtual VstInt32 processEvents (VstEvents* events);
		virtual bool processVariableIo ( VstVariableIo* varIo);
		virtual VstInt32 reportCurrentPosition ();
		virtual float* reportDestinationBuffer ();
		virtual void setBlockSizeAndSampleRate (VstInt32 blockSize, float sampleRate);

		virtual bool setSpeakerArrangement (VstSpeakerArrangement* pluginInput, VstSpeakerArrangement *pluginOutput);
		//we have our own UI handling
		//virtual bool setViewPosition (long x, long y);


	private:
#ifndef MACX
		DWORD ProcessEventsThreadID;
#else
		pthread_t ProcessEventsThreadID;
#endif
		JNIEnv *ProcessEventsJEnv;

		void initProcessEventsCache();
		bool isProcessEventsCacheInitialised;

		//cached fields
		jmethodID ProcessEventsMethodID;
		jmethodID EventConstructorMethodID;
		jmethodID MidiEventConstructorMethodID;

		jclass VSTEventsClass;
		jclass VSTEventClass;
		jclass VSTMidiEventClass;

		jobject VSTEventsObject;

		jfieldID VSTEventsFieldEvents;
		jfieldID VSTEventsFieldNumEvents;
		jfieldID VSTEventsFieldReserved;

		jfieldID VSTEventFieldType;
		jfieldID VSTEventFieldByteSize;
		jfieldID VSTEventFieldDeltaFrames;
		jfieldID VSTEventFieldFlags;
		jfieldID VSTEventFieldData;
		jfieldID VSTMidiEventFieldNoteLength;
		jfieldID VSTMidiEventFieldNoteOffset;
		jfieldID VSTMidiEventFieldDetune;
		jfieldID VSTMidiEventFieldNoteOffVelocity;
		jfieldID VSTMidiEventFieldReserved1;
		jfieldID VSTMidiEventFieldReserved2;


		void initVarIoCache();
		bool isVarIoCacheInitialised;

		jmethodID ProcessVarIoMethodID;

		jclass VarIoClass;
		jobject VarIoObject;

		jfieldID VarIoFieldInputs;
		jfieldID VarIoFieldOutputs;
		jfieldID VarIoFieldNumSamplesInput;
		jfieldID VarIoFieldNumSamplesOutput;
		jfieldID VarIoFieldNumSamplesInputProcessed;
		jfieldID VarIoFieldNumSamplesOutputProcessed;



		void initSpeakerCache();
		bool isSpeakerCacheInitialised;

		jclass jSpeakerArrClass;		
		jclass jSpeakerPropsClass;

		jmethodID SpeakerPropsConstructor;
		jmethodID SetSpeakerArrMethod;

		jobject jSpeakerArrInObject;
		jobject jSpeakerArrOutObject;

		jfieldID typeField;
		jfieldID numChannelsField;
		jfieldID speakersField;
			
		jfieldID azimuthField;
		jfieldID elevationField;
		jfieldID radiusField;
		jfieldID reservedField;
		jfieldID nameField;
		jfieldID propsTypeField;
		jfieldID futureField;

};



#endif