//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Definitions for the Java VSTi Wrapper class
//-
// 2003,2004 Daniel Martin
//-------------------------------------------------------------------------------------------------------

#ifndef __VSTV24ToPlug__
#define __VSTV24ToPlug__


#ifndef __VSTV23ToPlug__
#include "VSTV23ToPlug.h"
#endif




//------------------------------------------------------------------------
class VSTV24ToPlug : public VSTV23ToPlug {

	public:
		VSTV24ToPlug (audioMasterCallback audioMaster, int prg, int prm, JavaVM *jvm);
		virtual ~VSTV24ToPlug ();


		//VST2.4 Required
		//Nothing


		//VST2.4 Optional
		//******************************
		virtual bool setProcessPrecision (VstInt32 precision);
		///< Set floating-point precision used for processing (32 or 64 bit)

		virtual VstInt32 getNumMidiInputChannels();			
		///< Return number of MIDI input channels
		virtual VstInt32 getNumMidiOutputChannels();			
		///< Return number of MIDI output channels

		virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);
		///< Process 64 bit (double precision) floats (always in a resume state) \sa processReplacing


	protected:
		jclass JavaDoubleClass;


	private:
		jmethodID ProcessDoubleReplacingMethodID;
		JNIEnv *ProcessDoubleReplacingJEnv;
		
#ifndef MACX
		DWORD ProcessDoubleReplacingThreadID;
#else
		int ProcessDoubleReplacingThreadID;
#endif
};



#endif