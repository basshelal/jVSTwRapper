//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Wrapper for VSTi GUI
//-
// 2003,2004 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------

#ifndef __VSTGUIWrapper__
#include "VSTGUIWrapper.h"
#endif

#ifndef __VSTV10ToPlug__
#include "VSTV10ToPlug.h"
#endif

#ifndef __ConfigFileReader__
#include "ConfigFileReader.h"
#endif

#ifndef __JNIUtils__
#include "JNIUtils.h"
#endif

#ifndef MACX
#include <windows.h>
#else
#include <pthread.h>
#endif


VSTGUIWrapper::VSTGUIWrapper (AudioEffect *effect) 
	: AEffGUIEditor (effect) {

	this->ThreadID = 0;
	this->Jvm = ((VSTV10ToPlug*)effect)->Jvm;
	this->JEnv = ((VSTV10ToPlug*)effect)->JEnv;
	this->JavaPlugObj = ((VSTV10ToPlug*)effect)->JavaPlugObj;
}

//-----------------------------------------------------------------------------
VSTGUIWrapper::~VSTGUIWrapper () {
	this->ensureJavaThreadAttachment();
	
	//destroy() der gui aufrufen
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "destroy", "()V");
	if (mid == NULL) log("** ERROR: cannot find GUI instance-method destroy()V");
	
	this->JEnv->CallVoidMethod(this->JavaPlugGUIObj, mid);

	this->checkException();

	//free global referencw
	this->JEnv->DeleteGlobalRef(this->JavaPlugGUIObj);

	log("GUI Wrapper destroyed!");
}

//-----------------------------------------------------------------------------
long VSTGUIWrapper::open (void *ptr) {
	this->ensureJavaThreadAttachment();
	
	// !!! always call this !!!
	//AEffGUIEditor::open (ptr);

	//close the native plugin window
	//-->we use our own
	
	
#ifndef MACX
	ConfigFileReader *cfg = new ConfigFileReader();

	if (cfg->CloseNativePluginWindow==1)
		DestroyWindow(GetParent((HWND)ptr));	

	if (cfg) delete cfg;
#endif
	
	/*
	This would be the point, to 'inject' the hwnd from 
	the native app (plugin window) into an swing (awt) frame 
	via jni calls, to have the swing window embedded (as mdi) 
	into the native audio app.

    use sun.com.awt.WEmbeddedFrame to embedd into native app
	this class takes a hwnd as constructor argument

	NOTE:
	this is an internal class and may change without notice. 
	If we use this class, were bound to AWT AND to a specific 
	release of the Sun JVM. This kills portability of the wrapper, 
	so the strategy would be, check for the WEmbeddedFrame class first (try to load 
	it via a native call...), if its there try to use it... on any error, 
	fall back to the normal swing use.


	NOTE2:
	There must be a better way than destroying the native window (sometimes this kills 
	the entire host app). so, the plan here would be, install a window handler, 
	everytime there is a show message on the native handle, do a hide.
	I need to look at the winapi window proc thing again...
	

    
    I know, that every swing or awt window keeps a native hwnd in  
	a private member variable... privates are accesible via jni, so 
	maybe heres the point to inject the native hwnd to the swing window...

	
	I am still looking how to do this... Anyone got some suggestions?

	any comments on that are highly appreciated --> daniel309@users.sourceforge.net
	*/



	//this code works everywhere except in cubase sx!
	/*
	SendMessage((HWND)ptr, WM_CLOSE, 0, 0);
	PostQuitMessage(0);
	*/

	//PostQuitMessage(WM_QUIT);

	//SendMessage((HWND)ptr, WS_MINIMIZE, 0, 0);
	//ShowWindow(GetParent((HWND)ptr), SW_HIDE);

	//PostMessage((HWND)ptr, WM_CLOSE, 0, 0);
	//CloseWindow((HWND)ptr);
	//DestroyWindow((HWND)ptr);

	//AEffGUIEditor::close();

	/*
	SendMessage((HWND)ptr, WM_DESTROY, 0, 0);
	SendMessage((HWND)ptr, WM_QUIT, 0, 0);
    
	PostMessage((HWND)ptr, WM_DESTROY, 0, 0);
	PostMessage((HWND)ptr, WM_QUIT, 0, 0);
	*/

	//hier java swing gui open aufrufen...
	//frame.show()
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "open", "()V");
	if (mid == NULL) log("** ERROR: cannot find GUI instance-method open()V");
	
	this->checkException();

	this->JEnv->CallVoidMethod(this->JavaPlugGUIObj, mid);

	this->checkException();

	return true;
}

//-----------------------------------------------------------------------------
void VSTGUIWrapper::close () {
	this->ensureJavaThreadAttachment();

	//hier swing gui close aufrufen...
	//frame.hide();
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "close", "()V");
	if (mid == NULL) log("** ERROR: cannot find GUI instance-method close()V");
	
	this->JEnv->CallVoidMethod(this->JavaPlugGUIObj, mid);

	this->checkException();
}






//-----------------------------------------------------------------------------
int VSTGUIWrapper::initJavaSide(jclass guiClass) {
	this->ensureJavaThreadAttachment();

	if (guiClass==NULL) return -1;
	this->JavaPlugGUIClass = guiClass;


	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find GUIs default contructor");
		this->checkException(); //print stack trace!
		return -1;
	}

	if (this->checkException()) return -1;

	this->JavaPlugGUIObj = JEnv->NewObject(this->JavaPlugGUIClass, mid);
	if (this->JavaPlugGUIObj == NULL) {
		log("** ERROR: cannot create Java Plugin GUI Object");
		this->checkException(); //print stack trace!
		return -1;
	}

	if (this->checkException()) return -1;
	
	//create global reference --> this obj is used accross different threads
	this->JavaPlugGUIObj = this->JEnv->NewGlobalRef(this->JavaPlugGUIObj);

	if (this->checkException()) return -1;

	//Java GUI Obj .init(effect e) aufrufen
	mid = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "init", "(Ljvst/wrapper/VSTPluginAdapter;)V");
	if (mid == NULL) {
		log("** ERROR: cannot find GUI instance-method init(Ljvst/wrapper/VSTPluginAdapter;)V");
		this->checkException(); //print stack trace!
		return -1;
	}
	this->JEnv->CallVoidMethod(this->JavaPlugGUIObj, mid, this->JavaPlugObj);

	if (this->checkException()) return -1;
	else return 0;
}


//-----------------------------------------------------------------------------
void  VSTGUIWrapper::ensureJavaThreadAttachment() {
	this->checkException(); //if theres a pending exception, print stack trace & clear it!
	
#ifndef MACX	
	if (this->ThreadID != GetCurrentThreadId()) {
		this->ThreadID = GetCurrentThreadId();
#else
		if (!pthread_equal(pthread_self(),this->ThreadID)){
			this->ThreadID = pthread_self();
#endif	
	

		long stat = this->Jvm->AttachCurrentThread((void**)&this->JEnv, NULL);
		if (stat<0) log("** ERROR: attaching to THREAD in GUI Wrapper!");

		char temp[100];
		sprintf(temp, "New GUI wrapper ThreadID=%i", this->ThreadID);
		log(temp);
	}
}


bool VSTGUIWrapper::checkException() {
	return ::checkException(this->JEnv);	
}