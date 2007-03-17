/* 
 * jVSTwRapper - The Java way into VST world!
 * 
 * jVSTwRapper is an easy and reliable Java Wrapper for the Steinberg VST interface. 
 * It enables you to develop VST 2.3 compatible audio plugins and virtual instruments 
 * plus user interfaces with the Java Programming Language. 3 Demo Plugins(+src) are included!
 * 
 * Copyright (C) 2006  Daniel Martin [daniel309@users.sourceforge.net] 
 * 					   and many others, see CREDITS.txt
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

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
		
#ifdef WIN32
		DWORD ProcessDoubleReplacingThreadID;
#endif
#ifdef MACX
		pthread_t ProcessDoubleReplacingThreadID;
#endif
};



#endif