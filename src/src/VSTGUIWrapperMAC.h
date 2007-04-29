//
//  VSTGUIWrapperMAC.h
//  jvstwrapper
//
//  Created by Daniel Martin on 29.12.06.
//  Copyright 2006 __MyCompanyName__. All rights reserved.
//

#ifndef __VSTGUIWrapperMAC__
#define __VSTGUIWrapperMAC__


//objc wrapper functions
int initializeCocoa();
OSStatus testme123();

enum MethodsToPerformOnThread {
	GuiWrapperInitJavaSide = 1,
	GuiWrapperOpen,
	GuiWrapperClose, 
	GuiWrapperDestroy,
	GuiWrapperInitCocoa,
	GuiWrapperDummy
};

/*
int performOnAnotherThread(void* obj, void* param, int method, bool mainThread);
void printAppKitThreadID();

void aquireGlobalLock();
void releaseGlobalLock();

void aquireInitializerLock();
void releaseInitializerLock();
*/
#endif