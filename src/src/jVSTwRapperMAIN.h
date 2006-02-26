//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Dll WinAPI entry
// VSTi API entry
//
// 2003,2004 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------

#ifndef __jVSTwRapperMAIN__
#define __jVSTwRapperMAIN__


#ifndef __audioeffectx__
#include "public.sdk/vst2.x/audioeffectx.h"
#endif


//------------------------------------------------------------------------
// Prototype of the export functions
//------------------------------------------------------------------------

#ifndef MACX
	AEffect *main (audioMasterCallback audioMaster);
#else
	extern "C" AEffect *main_macho (audioMasterCallback audioMaster);
#endif




#endif
