//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// A JNI Wrapper 
// for Java as the language to write VSTi Plugins
//-------------------------------------------------------------------------------------------------------

	
#ifndef __VSTV22ToPlug__
#include "VSTV22ToPlug.h"
#endif

#ifndef __JNIUtils__
#include "JNIUtils.h"
#endif




//Impl of Host -> Plug calls
//Native -> Java 
//-----------------------------------------------------------------------------
VSTV22ToPlug::VSTV22ToPlug (audioMasterCallback audioMaster, int progs, int parms, JavaVM *jvm) 
	: VSTV21ToPlug (audioMaster, progs, parms, jvm) {



}

//-----------------------------------------------------------------------------------------
VSTV22ToPlug::~VSTV22ToPlug () {

}