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

#ifndef __VSTV22ToPlug__
#define __VSTV22ToPlug__

#include "VSTV21ToPlug.h"


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
