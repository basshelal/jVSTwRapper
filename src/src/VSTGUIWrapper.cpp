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
// Wrapper for VSTi GUI
//-
// 2003,2004 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------


#include "VSTGUIWrapper.h"
#include "VSTV10ToPlug.h"
#include "ConfigFileReader.h"
#include "JNIUtils.h"


#if defined(WIN32) || defined(linux)
	#include "jawt_md.h"
#endif
#if defined(MACX) || defined(linux)
	#include <pthread.h>
#endif
#ifdef MACX
	#include "VSTGUIWrapperMAC.h"
#endif
#ifdef WIN32
	#include <windows.h>
#endif
#ifdef linux
	#include <unistd.h>
#endif



VSTGUIWrapper::VSTGUIWrapper (AudioEffect *effect) 
	: AEffGUIEditor (effect) {

	log("GUI wrapper init");

	this->ThreadID = 0;	
	this->Jvm = ((VSTV10ToPlug*)effect)->Jvm;
	this->JEnv = ((VSTV10ToPlug*)effect)->JEnv;
	this->JavaPlugObj = ((VSTV10ToPlug*)effect)->JavaPlugObj;

#if defined (WIN32) || defined(linux)
	this->JavaWindowHandle = 0;

	ConfigFileReader *cfg = new ConfigFileReader();
	if(cfg!=NULL) {
	  this->AttachWindow=(cfg->AttachToNativePluginWindow==1);
	  delete cfg;
	}
#endif
#ifdef MACX
	//no window embedding on the mac for now...
	this->AttachWindow=0;
#endif
}

//-----------------------------------------------------------------------------
bool VSTGUIWrapper::getRect (ERect **ppErect) {
	log("GUI wrapper getRect");
	if(this->AttachWindow) {
		this->ensureJavaThreadAttachment();

		jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "getWidth", "()I");
		if (mid == NULL) log("** ERROR: cannot find GUI instance-method getWidth()I");
		jint width=this->JEnv->CallIntMethod(this->JavaPlugGUIObj, mid);
		this->checkException();

		mid = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "getHeight", "()I");
		if (mid == NULL) log("** ERROR: cannot find GUI instance-method getHeight()I");
		jint height=this->JEnv->CallIntMethod(this->JavaPlugGUIObj, mid);
		this->checkException();

		rect.left   = 0;
		rect.top    = 0;
		rect.right  = (short)width;
		rect.bottom = (short)height;
	} 
	else {
		rect.left   = 0;
		rect.top    = 0;
		rect.right  = 1; //maybe this fixes the fact that some hosts dont even call open() ?
		rect.bottom = 1;
	}
	*ppErect = &rect;
	return true;
}

//-----------------------------------------------------------------------------
VSTGUIWrapper::~VSTGUIWrapper () {
	log("GUI wrapper destroy");
	
	this->ensureJavaThreadAttachment();
	
	//destroy() der gui aufrufen
	//macx tends to block forever here...
	//BUT WE SOLVE THIS ON THE JAVA SIDE!
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "destroy", "()V");
	if (mid == NULL) log("** ERROR: cannot find GUI instance-method destroy()V");
	
	this->JEnv->CallVoidMethod(this->JavaPlugGUIObj, mid);

	this->checkException();

	//free global referencw
	this->JEnv->DeleteGlobalRef(this->JavaPlugGUIObj);

	log("GUI Wrapper destroyed!");
}


//-----------------------------------------------------------------------------

#ifdef WIN32
static WNDPROC oldWndProcEdit;
LONG WINAPI WindowProcEdit (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LONG WINAPI WindowProcEdit (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {	
	switch (message) {
		case WM_ERASEBKGND : {		
			CFrame* frame = (CFrame*)GetWindowLong(hwnd, GWL_USERDATA);
			HWND javaHandle = ((VSTGUIWrapper*)frame->getEditor())->JavaWindowHandle;
			RedrawWindow(javaHandle, NULL, NULL, RDW_NOERASE);
			return 1;
		}	
	}
	return CallWindowProc (oldWndProcEdit, hwnd, message, wParam, lParam);
}
#endif
#ifdef linux
	Display *GlobalDisplay = NULL;
	
int errorHandler(Display *dp, XErrorEvent *e) {
	char text[1024];
	XGetErrorText(dp, e->error_code, text, sizeof(text));
	text[1023]='\0';
	log("**XError: code=%i, minor=%i, request=%i, ressource=%i, serial=%i, type=%i, text=%s",
		e->error_code, e->minor_code, e->request_code, e->resourceid, e->serial, e->type, text);
	return 0;
}
#endif



bool VSTGUIWrapper::open (void *ptr) {
	log("GUI wrapper open %p", ptr);
	
	//!!! always call this !!!
	AEffGUIEditor::open(ptr);
	this->ensureJavaThreadAttachment();
	
	
#ifdef linux
	//install error handler to be notified if anything goes wrong in the win embedding code...
	XSetErrorHandler(errorHandler);
#endif

#if defined(WIN32) || defined(linux)
	ConfigFileReader *cfg = new ConfigFileReader();
	JAWT awt;	
	jboolean result;
	bool isAttached = false;

	if (this->AttachWindow && ptr!=NULL) {  
		log("attaching");
		this->AttachWindow=false; //if something goes wrong, dont try again
		jfieldID libraryOk = this->JEnv->GetStaticFieldID(this->JavaPlugGUIClass, "libraryOk", "Z");	
		this->checkException();
	   
		if (libraryOk != NULL) {
			jboolean lOk = this->JEnv->GetStaticBooleanField(this->JavaPlugGUIClass,libraryOk);
			this->checkException();
		   
			if(lOk==JNI_TRUE) {  
				log("successfully loaded the jawt library");
				// Get the AWT
				awt.version = JAWT_VERSION_1_3;
				result = JAWT_GetAWT(this->JEnv, &awt);
				
				if(result != JNI_FALSE) { 
					log("JAWT structs initialised!");
					//Inform the class
					jfieldID attachField = this->JEnv->GetFieldID(this->JavaPlugGUIClass, "WindowAttached", "Z");
					if (attachField != NULL) this->JEnv->SetBooleanField(this->JavaPlugGUIObj, attachField, JNI_TRUE);
					this->checkException();
					
					isAttached=true;
					this->AttachWindow=true;
				}
				else log("** Error: JAWT_GetAWT returned with error!");
			}
			else log("** Error: Could not load the JAWT library on the java side!");
		}
	}	
	else this->AttachWindow=false; //ptr was NULL, dont trust this host any more!


	if ((!isAttached) && (cfg->CloseNativePluginWindow==1)) {
#ifdef WIN32
		DestroyWindow(GetParent((HWND)ptr));
#endif
#ifdef linux
		/* Not implemented on Linux! (no way to obtain a ref to the diplay at this time...) */
		//if (::GlobalDisplay!=NULL) 
			//XDestroyWindow(::GlobalDisplay, (Window)ptr);
			//XUnmapWindow(::GlobalDisplay, (Window)ptr);
#endif	
	} 
	if (cfg) delete cfg;


	if(isAttached) {
        // Call Undecorate
		this->undecorateJavaWindow();

		//Try to attach the Window
		log("Attaching Window");
		JAWT_DrawingSurface* ds;
		JAWT_DrawingSurfaceInfo* dsi;
#ifdef WIN32
		JAWT_Win32DrawingSurfaceInfo* dsi_win;
#endif
#ifdef linux
		JAWT_X11DrawingSurfaceInfo* dsi_win;
#endif
		jint lock;				
  	    // Get the drawing surface
		ds = awt.GetDrawingSurface(this->JEnv, this->JavaPlugGUIObj);
		if(ds != NULL){
			log("obtained drawing surface");
			// Lock the drawing surface
			lock = ds->Lock(ds);
			if((lock & JAWT_LOCK_ERROR) == 0) {
				log("locked the drawing surface");
   				// Get the drawing surface info
				dsi = ds->GetDrawingSurfaceInfo(ds);
			
				if(dsi!=NULL) {
					log("obtained ds info");
					// Get the platform-specific drawing info
#ifdef WIN32
					dsi_win = (JAWT_Win32DrawingSurfaceInfo*)dsi->platformInfo;
#endif
#ifdef linux
					dsi_win = (JAWT_X11DrawingSurfaceInfo*)dsi->platformInfo;
#endif

//#ifdef WIN32
					//Create Frame to embedd the java Frame
					ERect* thissize;
					this->getRect(&thissize);
					
					//create new Frame using VSTGUI! (CRect, CFrame)
					CRect size (0, 0, thissize->right, thissize->bottom);

					//frame is a var from AEffectGUIEditor
					if (frame!=NULL) delete frame;
					frame = new CFrame (size, ptr, this);
					
#ifdef WIN32
					HWND frhwnd=(HWND)frame->getSystemWindow();

					//Get Java Window-Handle
					this->JavaWindowHandle=dsi_win->hwnd;
					//Set Parent Window
					SetParent((HWND)JavaWindowHandle, (HWND)frhwnd);
					
					//Set Windows Styles
					long style;
					style=(LONG)GetWindowLong(((HWND)frhwnd), GWL_STYLE);
					SetWindowLong((HWND)frhwnd, GWL_STYLE, style | WS_CLIPCHILDREN);

					style=(LONG)GetWindowLong(((HWND)ptr), GWL_STYLE);
					SetWindowLong((HWND)ptr, GWL_STYLE, style | WS_CLIPCHILDREN);

					style=GetWindowLong((HWND)JavaWindowHandle, GWL_STYLE);
					SetWindowLong(dsi_win->hwnd, GWL_STYLE, style | WS_CHILD);

					//Set new Paint-Method	
					oldWndProcEdit = (WNDPROC)SetWindowLong ((HWND)frhwnd, GWL_WNDPROC, (long)WindowProcEdit);
#endif
#ifdef linux
					Window frhwnd = (Window)frame->getSystemWindow(); 
					
					//Get Java Window-Handle
					this->JavaWindowHandle=dsi_win->drawable;  //cast drawable to window! (is only a handle anyways...)
					if (::GlobalDisplay==NULL) ::GlobalDisplay = dsi_win->display;   //init global var diplay
					
					//usleep(500); //wait a little to ensure that the x operation completes...
					
					//Set Parent Window
					int ret = XReparentWindow (::GlobalDisplay, this->JavaWindowHandle, frhwnd, 0, 0);
					//int ret = XReparentWindow (::GlobalDisplay, this->JavaWindowHandle, (Window)ptr, 0, 0);
					log("win reparent=%i", ret);
					
					//usleep(500); //wait a little to ensure that the x operation completes...
#endif

					// Free the drawing surface info
					ds->FreeDrawingSurfaceInfo(dsi);
				}
				else log("**Error: Unable to retrieve drawing surface INFO!");
 
				// Unlock the drawing surface
				ds->Unlock(ds);
			}
			else log("**Error: Unable to LOCK the drawing surface!");
			
			// Free the drawing surface
			awt.FreeDrawingSurface(ds);
		} 
		else log("**Error: Unable to retrieve the drawing surface!");
		
	} //Attaching    
#endif		

		
	// Call Open
    jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "open", "()V");
	if (mid == NULL) log("** ERROR: cannot find GUI instance-method open()V");
	this->JEnv->CallVoidMethod(this->JavaPlugGUIObj, mid);


#if defined(WIN32) || defined(linux)
	//Check exceptions in open, if exception, then unattach immediately
	if(this->JEnv->ExceptionCheck()==JNI_TRUE) {
		//If a Exception occured close the gui again
		log("Exception in Open!");
		this->checkException(); //log and clear exception
			
		this->AttachWindow=false; //dont attach again!
			
		if (isAttached) {
	        //Close Gui
			jmethodID midClose = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "close", "()V");
		    if (midClose == NULL) log("** ERROR: cannot find GUI instance-method close()V");
		    this->JEnv->CallVoidMethod(this->JavaPlugGUIObj, midClose);
			this->checkException();
	
			log("* WARNING: Exception occured in GUI open() --> disabling native window attachment");
			this->detachWindow(); //detach window	
	
			this->undecorateJavaWindow(); //call undecorate (this now redecorates the window)
	
			//Call open again
	 	    this->JEnv->CallVoidMethod(this->JavaPlugGUIObj, mid);
		}
	}
#endif

	this->checkException();
	return true;
}

//-----------------------------------------------------------------------------
void VSTGUIWrapper::close () {
	log("closing GUI");

	this->ensureJavaThreadAttachment();
    
#if defined(linux)
	this->detachWindow(); //on linux, first detach, then close
#endif
    
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "close", "()V");
	if (mid == NULL) log("** ERROR: cannot find GUI instance-method close()V");
	this->JEnv->CallVoidMethod(this->JavaPlugGUIObj, mid);
	bool error = this->checkException();

#if defined(WIN32) 
	this->detachWindow(); //on windows do the other way round!
#endif

#if defined(WIN32) || defined(linux) 
	if (error) {
		this->AttachWindow=false; //error in close --> dont attach to window again!
		log("* WARNING: Exception occured in GUI close() --> disabling native window attachment");
		this->undecorateJavaWindow(); //redecorate the unattached window
	}
#endif
}





//-----------------------------------------------------------------------------
int VSTGUIWrapper::initJavaSide(jclass guiClass) {
	log("GUI wrapper initJavaSide");
	this->ensureJavaThreadAttachment();

	if (guiClass==NULL) return -1;
	this->JavaPlugGUIClass = guiClass;

	log("WITHIN gui initjavaside");
	jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "<init>", "()V");	
	if (mid == NULL) {
		log("** ERROR: cannot find GUIs default contructor");
		this->checkException(); //print stack trace!
		return -1;
	}
	if (this->checkException()) return -1;
	
	log("creating instance of GUI class");

	this->JavaPlugGUIObj = this->JEnv->NewObject(this->JavaPlugGUIClass, mid);
	if (this->JavaPlugGUIObj == NULL) {
		log("** ERROR: cannot create Java Plugin GUI Object");
		this->checkException(); //print stack trace!
		return -1;
	}

	if (this->checkException()) return -1;
	
	//create global reference --> this obj is used accross different threads
	this->JavaPlugGUIObj = this->JEnv->NewGlobalRef(this->JavaPlugGUIObj);

	if (this->checkException()) return -1;

	log("calling GUI .init()");
	//Java GUI Obj .init(effect e) aufrufen
	mid = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "init", "(Ljvst/wrapper/VSTPluginAdapter;)V");
	if (mid == NULL) {
		log("** ERROR: cannot find GUI instance-method init(Ljvst/wrapper/VSTPluginAdapter;)V");
		this->checkException(); //print stack trace!
		return -1;
	}
	this->JEnv->CallVoidMethod(this->JavaPlugGUIObj, mid, this->JavaPlugObj);

	log("GUI initJavaSide OK!");

	if (this->checkException()) return -1;
	else return 0;
}


//-----------------------------------------------------------------------------
void  VSTGUIWrapper::ensureJavaThreadAttachment() {
	
#ifdef WIN32	
	if (this->ThreadID != GetCurrentThreadId()) {
		this->ThreadID = GetCurrentThreadId();
#endif
#if defined(MACX) || defined(linux)
		if (!pthread_equal(pthread_self(),this->ThreadID)){
			this->ThreadID = pthread_self();
#endif	

		long stat = this->Jvm->AttachCurrentThread((void**)&this->JEnv, NULL);
		if (stat<0) log("** ERROR: attaching to THREAD in GUI Wrapper!");

		log("New GUI wrapper ThreadID=%i", this->ThreadID);
	}
  	this->checkException(); //if theres a pending exception, print stack trace & clear it!
}

bool VSTGUIWrapper::checkException() {
	return ::checkException(this->JEnv);	
}


#if defined(WIN32) || defined(linux)
void VSTGUIWrapper::detachWindow() {
	log("detaching java window from native win");

	// Detach the Window
	if(this->JavaWindowHandle!=0) {   
#ifdef WIN32
		SetParent(this->JavaWindowHandle,NULL);
		this->JavaWindowHandle=NULL;
#endif
#ifdef linux
		//usleep(500); //wait a little to ensure that the x operation completes...
		int ret = XReparentWindow (::GlobalDisplay, this->JavaWindowHandle, DefaultRootWindow(::GlobalDisplay), 0, 0);
		usleep(500); //wait a little to ensure that the x operation completes...
		
		log("remap=%i", ret);
		this->JavaWindowHandle=0;
#endif
	}
	if(frame!=NULL) {
		delete frame;
		frame=NULL;
	}

	//Inform the class
	jfieldID attachField = this->JEnv->GetFieldID(this->JavaPlugGUIClass, "WindowAttached", "Z");
	if (attachField != NULL) this->JEnv->SetBooleanField(this->JavaPlugGUIObj, attachField, JNI_FALSE);
	this->checkException();
}

void VSTGUIWrapper::undecorateJavaWindow() {
	log("undecorate / redecorating Java window");
	// Call Undecorate
    jmethodID midUndeco = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "undecorate", "()V");
    if (midUndeco == NULL) log("** ERROR: cannot find GUI instance-method undecorate()V");
    this->JEnv->CallVoidMethod(this->JavaPlugGUIObj, midUndeco);
    this->checkException();
}
#endif

