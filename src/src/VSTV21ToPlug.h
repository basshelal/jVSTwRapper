//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Definitions for the Java VSTi Wrapper class
//-
// 2003,2004 Daniel Martin
//-------------------------------------------------------------------------------------------------------

#ifndef __VSTV21ToPlug__
#define __VSTV21ToPlug__


#ifndef __VSTV20ToPlug__
#include "VSTV20ToPlug.h"
#endif




//------------------------------------------------------------------------
class VSTV21ToPlug : public VSTV20ToPlug {

	public:
		VSTV21ToPlug (audioMasterCallback audioMaster, int prg, int prm, JavaVM *jvm);
		virtual ~VSTV21ToPlug ();

		//VST 2.1 Required
		//nothing


		//VST2.1 Optional
		//**********************

		virtual long getMidiProgramName (long channel, MidiProgramName* midiProgramName);
		// Struct will be filled with information for 'thisProgramIndex'.
		// returns number of used programIndexes.
		// If 0 is returned, no MidiProgramNames supported.
		
		virtual long getCurrentMidiProgram (long channel, MidiProgramName* currentProgram);
		// Struct will be filled with information for the current program.
		// Returns the programIndex of the current program. -1 means not supported.
		
		virtual long getMidiProgramCategory (long channel, MidiProgramCategory* category);
		// Struct will be filled with information for 'thisCategoryIndex'.
		// returns number of used categoryIndexes. 
		// if 0 is returned, no MidiProgramCategories supported/used.
		
		virtual bool hasMidiProgramsChanged (long channel);
		// Returns true if the MidiProgramNames, MidiKeyNames or 
		// MidiControllerNames had changed on this channel.
		
		virtual bool getMidiKeyName (long channel, MidiKeyName* keyName);
		// Struct will be filled with information for 'thisProgramIndex' and 'thisKeyNumber'
		// if keyName is "" the standard name of the key will be displayed.
		// if false is returned, no MidiKeyNames defined for 'thisProgramIndex'.

		virtual bool beginSetProgram ();
		// Called before a program is loaded

		virtual bool endSetProgram ();  
		// Called after...

};


#endif