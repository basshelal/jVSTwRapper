//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Definitions for the Java VSTi Wrapper class
//-
// 2003,2004 Daniel Martin
//-------------------------------------------------------------------------------------------------------

#ifndef __VSTV23ToPlug__
#define __VSTV23ToPlug__


#ifndef __VSTV22ToPlug__
#include "VSTV22ToPlug.h"
#endif




//------------------------------------------------------------------------
class VSTV23ToPlug : public VSTV22ToPlug {

	public:
		VSTV23ToPlug (audioMasterCallback audioMaster, int prg, int prm, JavaVM *jvm);
		virtual ~VSTV23ToPlug ();

		//VST2.3 Required
		//Nothing


		//VST2.3 Optional
		//******************************

		virtual VstInt32 setTotalSampleToProcess (VstInt32 value);
		// Called in offline (non RealTime) Process before process is called, indicates how many sample will be processed

		virtual VstInt32 getNextShellPlugin (char* name);
		// This opcode is only called, if Plugin is of type kPlugCategShell.
		// should return the next plugin's uniqueID.
		// name points to a char buffer of size 64, which is to be filled
       	// with the name of the plugin including the terminating zero.
		
		virtual VstInt32 startProcess ();
		// Called one time before the start of process call

		virtual VstInt32 stopProcess ();
		// Called after the stop of process call

};



#endif