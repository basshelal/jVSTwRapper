//
//  VSTGUIWrapperMAC.m
//  jvstwrapper
//
//  Created by Daniel Martin on 29.12.06.
//  Copyright 2006 http://www.jvstwrapper.sf.net All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Foundation/NSAutoreleasePool.h>
#import <jawt_md.h>

#import "VSTGUIWrapperMAC.h"
#import "JNIUtils.h"

#import <jni.h>

/*
@interface VSTGUIWrapperMAC : NSObject {

}
@end

@implementation VSTGUIWrapperMAC

	+ (void)awttest:(JNIEnv*) env {
		log("BEFORE TEST AREA!!!");

		jclass tkclass = env->FindClass("java/awt/Toolkit");
		if (tkclass == NULL) {
			log("** ERROR: could not load TOOLKIT class");
		}
		
		jmethodID tkmid = env->GetStaticMethodID(tkclass, "getDefaultToolkit", "()Ljava/awt/Toolkit;");
		if (tkmid == NULL) {
			log("** ERROR: CANNOT find TOOLKIT method");
		}
		
		env->CallStaticObjectMethod(tkclass, tkmid);

		log("AFTER TEST AREA!!!");
	}

	- (void)loadAWT:(JNIEnv*) env {
		[self performSelectorOnMainThread:@selector(awttest:) withObject:env waitUntilDone:YES];
	}

@end
*/


int initializeCocoa() {
	//create anonymous autorelease pool
	//this prevents the problem when loading NapkinGUI stuff
	//this generally fixes the bug that a program bocks 
	//when an image is loaded using awt methods.
	//this is a known bug in apples jvm implementation
	//comment this line out and load a gui that uses images... (e.g. napkinlaf)
	[[NSAutoreleasePool alloc] init];
	
	//init cocoa, so that we can interop with it in carbon
	//also, without this, we wouldnt be able to create any java window (awt/swing) 
	BOOL ret = NSApplicationLoad();
	
	return (ret==YES) ? 0 : -1;
}

int objectiveCwrapperTemplate() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	//do stuff
	
	[pool release];
	return 0;
}

OSStatus testme123() {
	printf("Hello from ObjC++!");
	return noErr;
}


