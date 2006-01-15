//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Definitions for the Java VSTi Wrapper class
//-
// 2003,2004 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------

#ifndef __VSTV10ToPlug__
#define __VSTV10ToPlug__


#ifndef __audioeffectx__
#include "vst/audioeffectx.h"
#endif

#ifndef __VSTGUIWrapper__
#include "VSTGUIWrapper.h"
#endif


#include <jni.h>


//------------------------------------------------------------------------
class VSTV10ToPlug : public AudioEffectX {
	friend class VSTGUIWrapper;

	public:
		VSTV10ToPlug (audioMasterCallback audioMaster, int prg, int prm, JavaVM *jvm);
		virtual ~VSTV10ToPlug ();


		//HOST -> PLUG


		//vst 1.0 REQUIRED
		//*****************
		virtual void setParameter (long index, float value);
		virtual float getParameter (long index);
		
		virtual void process (float **inputs, float **outputs, long sampleframes);
		virtual void processReplacing (float **inputs, float **outputs, long sampleFrames);
		
		virtual long getProgram();
		virtual void setProgram (long program);
		virtual void setProgramName (char *name);
		virtual void getProgramName (char *name);

		virtual void getParameterLabel (long index, char *label);
		virtual void getParameterDisplay (long index, char *text);
		virtual void getParameterName (long index, char *text);


		//vst 1.0 OPTIONAL
		//*****************
		virtual void resume ();	// Called when Plugin is switched to On
		virtual void suspend ();// Called when Plugin is switched to Off
		virtual void open ();
		virtual void close ();

		virtual float getVu();
		virtual long getChunk(void** data, bool isPreset = false);
		virtual long setChunk(void* data, long byteSize, bool isPreset = false);
		virtual void setBlockSize(long blockSize);
		virtual void setSampleRate(float sampleRate);

		
		//UTILITY
		int initJavaSide(jclass plug, bool hasGUI);


	protected:
		void ensureJavaThreadAttachment();
		bool checkException();

		JNIEnv *JEnv;
		JavaVM *Jvm;
		jobject JavaPlugObj;
		jclass JavaPlugClass;
		jclass JavaFloatClass;

	private:
		jmethodID ProcessMethodID;
		jmethodID ProcessReplacingMethodID;
		JNIEnv *ProcessJEnv;
		JNIEnv *ProcessReplacingJEnv;
		

#ifndef MACX
		DWORD ProcessThreadID;
		DWORD ProcessReplacingThreadID;
		DWORD ThreadID;
#else
		int ProcessThreadID;
		int ProcessReplacingThreadID;
		int ThreadID;
#endif		

		//cached methods ids;
		jmethodID GetParameterMethod;
		jmethodID SetParameterMethod;

		//cached pointer to chunk-data
		void* chunkdata;
		long chunksize;

};

#endif
