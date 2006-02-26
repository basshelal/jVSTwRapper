//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Wrapper for VSTi GUI
//-
// 2003,2004 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------

#ifndef __VSTGUIWrapper__
#define __VSTGUIWrapper__

#include <jni.h>

#ifndef __vstgui__
#include "vstgui/vstgui.h"
#endif


//-----------------------------------------------------------------------------
class VSTGUIWrapper : public AEffGUIEditor {

	public:
		VSTGUIWrapper (AudioEffect *effect);
		virtual ~VSTGUIWrapper ();

		virtual bool open (void *ptr);
		virtual void close ();
		virtual bool getRect (ERect **ppRect);
		
		//Utility
		int initJavaSide(jclass gui);	

#ifndef MACX
		HWND JavaWindowHandle;
#endif

	protected:
		bool checkException();


	private:
		void ensureJavaThreadAttachment();

#ifndef MACX
		DWORD ThreadID;
		
#else
		pthread_t ThreadID;
#endif

		JNIEnv *JEnv;
		JavaVM *Jvm;
		jobject JavaPlugObj;
		jobject JavaPlugGUIObj;
		jclass JavaPlugGUIClass;

		bool AttachWindow;

		// Bitmap
		//CBitmap *hBackground;
};


#endif

