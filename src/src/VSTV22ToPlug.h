//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Definitions for the Java VSTi Wrapper class
//-
// 2003,2004 Daniel Martin
//-------------------------------------------------------------------------------------------------------

#ifndef __VSTV22ToPlug__
#define __VSTV22ToPlug__


#ifndef __VSTV21ToPlug__
#include "VSTV21ToPlug.h"
#endif




//------------------------------------------------------------------------
class VSTV22ToPlug : public VSTV21ToPlug {

	public:
		VSTV22ToPlug (audioMasterCallback audioMaster, int prg, int prm, JavaVM *jvm);
		virtual ~VSTV22ToPlug ();


		//VST2.2 Required
		//Nothing


		//VST2.2 Optional
		//Nothing


};



#endif