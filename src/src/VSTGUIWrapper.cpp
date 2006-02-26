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

#include "jawt_md.h"

#ifndef MACX
#include <windows.h>
#else
#include <pthread.h>
#endif


VSTGUIWrapper::VSTGUIWrapper (AudioEffect *effect) 
	: AEffGUIEditor (effect) {

	this->ThreadID = 0;	
	this->JavaWindowHandle = 0;
	this->Jvm = ((VSTV10ToPlug*)effect)->Jvm;
	this->JEnv = ((VSTV10ToPlug*)effect)->JEnv;
	this->JavaPlugObj = ((VSTV10ToPlug*)effect)->JavaPlugObj;	
#ifndef MACX
	ConfigFileReader *cfg = new ConfigFileReader();
	if(cfg!=NULL)
	{
	  this->AttachWindow=(cfg->AttachToNativePluginWindow==1);
	  delete cfg;
	}
#else
	this->AttachWindow=0;
#endif
}

//-----------------------------------------------------------------------------
bool VSTGUIWrapper::getRect (ERect **ppErect)
{
	if(this->AttachWindow) {
	   //Set Size
	   this->ensureJavaThreadAttachment();
       //Get Width
	   jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "getWidth", "()I");
	   this->checkException();
	   if (mid == NULL) log("** ERROR: cannot find GUI instance-method getWidth()I");
	
	   jint width=this->JEnv->CallIntMethod(this->JavaPlugGUIObj, mid);

	   this->checkException();
       //Get Height
	   mid = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "getHeight", "()I");
	   this->checkException();
	   if (mid == NULL) log("** ERROR: cannot find GUI instance-method getHeight()I");
	
	   jint height=this->JEnv->CallIntMethod(this->JavaPlugGUIObj, mid);

	   this->checkException();

	   rect.left   = 0;
       rect.top    = 0;
	   rect.right  = (short)width;
       rect.bottom = (short)height;
	} else {
       rect.left   = 0;
       rect.top    = 0;
	   rect.right  = 0;
       rect.bottom = 0;
	}
	*ppErect = &rect;
	return true;
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

#ifndef MACX

static WNDPROC oldWndProcEdit;
LONG WINAPI WindowProcEdit (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LONG WINAPI WindowProcEdit (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	switch (message)
	{
		case WM_ERASEBKGND :
		{		
			CFrame* frame=(CFrame*)GetWindowLong(hwnd,GWL_USERDATA);
			HWND javaHandle=((VSTGUIWrapper*)frame->getEditor())->JavaWindowHandle;
			RedrawWindow(javaHandle,NULL,NULL,RDW_NOERASE);
			return 1;
		}	
	}
	return CallWindowProc (oldWndProcEdit, hwnd, message, wParam, lParam);
}
#endif

bool VSTGUIWrapper::open (void *ptr) {

    this->ensureJavaThreadAttachment();

#ifndef MACX
    ConfigFileReader *cfg = new ConfigFileReader();
	JAWT awt;	
	jboolean result;
	bool isAttached = false;
	if (this->AttachWindow){  
	   this->AttachWindow=false;
	   jfieldID libraryOk = this->JEnv->GetStaticFieldID(this->JavaPlugGUIClass, "libraryOk", "Z");	
	   this->checkException();
	   if (libraryOk != NULL) {
		   jboolean lOk = this->JEnv->GetStaticBooleanField(this->JavaPlugGUIClass,libraryOk);
		   this->checkException();
		   if(lOk==JNI_TRUE) {  
				// Get the AWT
				awt.version = JAWT_VERSION_1_3;
				result = JAWT_GetAWT(this->JEnv, &awt);
				if(result != JNI_FALSE)  { 
				  //Inform the class
				  jfieldID attachField = this->JEnv->GetFieldID(this->JavaPlugGUIClass, "WindowAttached", "Z");
   			      jboolean val=JNI_TRUE;
				  this->checkException();
                  if (attachField != NULL) 
				  this->JEnv->SetBooleanField(this->JavaPlugGUIObj,attachField,val);
				  this->checkException();    
				  isAttached=true;
				  this->AttachWindow=true;
				}
		   }
	   }
	}	
	if ((!isAttached)&&(cfg->CloseNativePluginWindow==1)) {
		DestroyWindow(GetParent((HWND)ptr));	
	} 
	if (cfg) delete cfg;

	if(isAttached) {
        // Call Undecorate
        jmethodID mid = this->JEnv->GetMethodID(this->JavaPlugGUIClass, "undecorate", "()V");
	    if (mid == NULL) log("** ERROR: cannot find GUI instance-method undecorate()V");
	
   	    this->checkException();

  	    this->JEnv->CallVoidMethod(this->JavaPlugGUIObj, mid);

	    this->checkException();

	   //Try to attach the Window
		log("Attach Winow");
		JAWT_DrawingSurface* ds;
		JAWT_DrawingSurfaceInfo* dsi;
		JAWT_Win32DrawingSurfaceInfo* dsi_win;
		
		jint lock;				
  	    // Get the drawing surface
		ds = awt.GetDrawingSurface(this->JEnv, this->JavaPlugGUIObj);
		if(ds != NULL){
          // Lock the drawing surface
		  lock = ds->Lock(ds);
		  if((lock & JAWT_LOCK_ERROR) == 0) {
   		    // Get the drawing surface info
			dsi = ds->GetDrawingSurfaceInfo(ds);
			if(dsi!=NULL) {
   			  // Get the platform-specific drawing info
			  dsi_win = (JAWT_Win32DrawingSurfaceInfo*)dsi->platformInfo;

			  //Create Frame to embedd the java Frame
			  ERect* thissize;
			  this->getRect(&thissize);
			  CRect size (
				0,
				0,
				thissize->right,
				thissize->bottom
			  );
			  if(frame!=NULL) delete frame;
			  frame = new CFrame (size, ptr, this);
			  HWND frhwnd=(HWND)frame->getSystemWindow();
			  //Get Java Window-Handle
			  this->JavaWindowHandle=dsi_win->hwnd;
			  //Set Parent Window
              SetParent((HWND)JavaWindowHandle,(HWND)frhwnd);
			  //Set Windows Styles
			  long style;
			  style=(LONG)GetWindowLong(((HWND)frhwnd),GWL_STYLE);
			  SetWindowLong((HWND)frhwnd,GWL_STYLE,style|WS_CLIPCHILDREN);

			  style=(LONG)GetWindowLong(((HWND)ptr),GWL_STYLE);
			  SetWindowLong((HWND)ptr,GWL_STYLE,style|WS_CLIPCHILDREN);
			  
			  style=GetWindowLong((HWND)JavaWindowHandle,GWL_STYLE);
			  SetWindowLong(dsi_win->hwnd,GWL_STYLE,style|WS_CHILD);

			  //Set new Paint-Method	
			  oldWndProcEdit = (WNDPROC)SetWindowLong ((HWND)frhwnd, GWL_WNDPROC, (long)WindowProcEdit);
			  
			  // Free the drawing surface info
              ds->FreeDrawingSurfaceInfo(dsi);
			}
 
            // Unlock the drawing surface
			ds->Unlock(ds);
		  }
 		  // Free the drawing surface
		  awt.FreeDrawingSurface(ds);
		} //Drawng Surface Ok!
	} //Attaching    
#endif		
	// Call Open
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

#ifndef MACX
	//Inform the class
	if(this->AttachWindow) {
	  jfieldID attachField = this->JEnv->GetFieldID(this->JavaPlugGUIClass, "WindowAttached", "Z");
   	  jboolean val=JNI_FALSE;
	  this->checkException();
      if (attachField != NULL) 
	  val=this->JEnv->GetBooleanField(this->JavaPlugGUIObj,attachField);
	  this->checkException();                
      if(val==JNI_TRUE) // Detach the Window
	  {   
		if(this->JavaWindowHandle!=NULL)
		{
			SetParent(this->JavaWindowHandle,NULL);
			this->JavaWindowHandle=NULL;
		}
		if(frame!=NULL) {
			delete frame;
		    frame=NULL;
		}	   
	  }
	}
#endif
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