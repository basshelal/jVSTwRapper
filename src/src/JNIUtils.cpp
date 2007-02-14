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
// JNI utility functions
//-
// 2003,2004 Daniel Martin
//-------------------------------------------------------------------------------------------------------

#ifndef __JNIUtils__
#include "JNIUtils.h"
#endif

#ifndef __VSTV20ToPlug__
#include "VSTV20ToPlug.h"
#endif

#ifndef __ConfigFileReader__
#include "ConfigFileReader.h"
#endif

#ifdef MACX
	#include <Carbon/Carbon.h>
	#include <sys/stat.h>
#endif

//------------------------------------------------------------------------
int IsLogEnabled = 1;
int MessageBoxCount = 0;


//------------------------------------------------------------------------
jvalue JNU_CallJavaMethod(	JNIEnv *env,
							jobject obj, 
							jmethodID mid, 
							const char *name,
							const char *descriptor, 
							...) {
	va_list args;
	jvalue result;
	char temp[70];

	sprintf(temp, "Invoking JNU_CallJavaMethod method=%s descriptor=%s", name, descriptor);
	log(temp);

	if (mid) {
		const char *p = descriptor;
		//skip over argument types to find out the 
		//return type 

		while (*p != ')') p++; // skip till ')'
		p++;
		va_start(args, descriptor);

		switch (*p) {
			case 'V':
				env->CallVoidMethodV(obj, mid, args);
				break;
			case '[':
			case 'L':
				result.l = env->CallObjectMethodV( obj, mid, args);
				break;
			case 'Z':
				result.z = env->CallBooleanMethodV(obj, mid, args);
				break;
			case 'B':
				result.b = env->CallByteMethodV(obj, mid, args);
				break;
			case 'C':
				result.c = env->CallCharMethodV(obj, mid, args);
				break;
			case 'S':
				result.s = env->CallShortMethodV(obj, mid, args);
				break;
			case 'I':
				result.i = env->CallIntMethodV(obj, mid, args);
				break;
			case 'J':
				result.j = env->CallLongMethodV(obj, mid, args);
				break;
			case 'F':
				result.f = env->CallFloatMethodV(obj, mid, args);
				break;
			case 'D':
				result.d = env->CallDoubleMethodV(obj, mid, args);
				break;
			default:
				env->FatalError("** JNU_CallJavaMethod: illegal descriptor");
		}
		
		va_end(args);
	}

	checkException(env);

	return result;
}

//------------------------------------------------------------------------
int log(char* data) {
    int retval = -1;
	int isWarningOrError = 1;    

	if (data==NULL) return retval;

	isWarningOrError = (*data == '*') && (*(data+1)=='*');

	//show MessageBox on Error or warning!
	//restrict it to 5 messages per session.
	//more would just be annoying...
	if (isWarningOrError) {
		MessageBoxCount++;	
		if (MessageBoxCount<5) {
#ifndef MACX
			MessageBoxA(0, data, "jVSTwRapper", 0);
#else
//Uses Carbon Alert window. But the window displayed doesnt receive any input. 
//this blocks the whole application here... so its commented out by now. 
//Maybe someone knows a solution?
/*
			DialogRef alertref = NULL;
			CFStringRef msg = CFStringCreateWithCString(NULL, data, kCFStringEncodingASCII);
			
			AlertStdCFStringAlertParamRec param;
			param.version=kStdCFStringAlertVersionOne;
			param.movable=true;
			param.helpButton=false;
			param.defaultText=(CFStringRef)kAlertDefaultOKText;
			param.cancelText=NULL;
			param.otherText=NULL;
			param.defaultButton=kAlertStdAlertOKButton;
			param.cancelButton=NULL;
			param.position=kWindowDefaultPosition;
			param.flags=0;
			
			CreateStandardAlert (kAlertCautionAlert, CFSTR("jVSTwRapper"), msg, &param, &alertref);
			RunStandardAlert (alertref, NULL, &buttonPressed);
*/
#endif
		}
	}

	if (IsLogEnabled || isWarningOrError) {
		fprintf(stderr, "\n");
		fprintf(stderr, data);
		fflush(stderr);
		retval = 0;
	} else retval = 0;

    return retval;
}


#define WHITESPACE_STR  " \f\n\r\t\v"
/**
 * Remove whitespace characters from both ends of a copy of
 *  '\0' terminated STRING and return the result.
 **/
char *trim (char *string) {
  char *result = 0;

  /* Ignore NULL pointers.  */
  if (string) {
      char *ptr = string;

      /* Skip leading whitespace.  */
      while (strchr (WHITESPACE_STR, *ptr))
        ++ptr;

      /* Make a copy of the remainder.  */
      result = strdup (ptr);

      /* Move to the last character of the copy.  */
      for (ptr = result; *ptr; ++ptr);
      --ptr;

      /* Remove trailing whitespace.  */
      for (--ptr; strchr (WHITESPACE_STR, *ptr); --ptr)
          *ptr = '\0';
   }

  return result;
}


char *replace(char *string, char *oldpiece, char *newpiece) { 
   int str_index, newstr_index, oldpiece_index, end, new_len, old_len, cpy_len; 
   char *c; 
   char newstring[4096]; 


   if ((c = (char *) strstr(string, oldpiece)) == NULL) 
      return string; 


   new_len = strlen(newpiece); 
   old_len = strlen(oldpiece); 
   end = strlen(string) - old_len; 
   oldpiece_index = c - string; 


   newstr_index = 0; 
   str_index = 0; 
   while(str_index <= end && c != NULL) { 
      /* Copy characters from the left of matched pattern occurence */ 
      cpy_len = oldpiece_index-str_index; 
      strncpy(newstring+newstr_index, string+str_index, cpy_len); 
      newstr_index += cpy_len; 
      str_index += cpy_len; 


      /* Copy replacement characters instead of matched pattern */ 
      strcpy(newstring+newstr_index, newpiece); 
      newstr_index += new_len; 
      str_index += old_len; 


      /* Check for another pattern match */ 
      if((c = (char *) strstr(string+str_index, oldpiece)) != NULL) 
         oldpiece_index = c - string; 
   } 
   /* Copy remaining characters from the right of last matched pattern */ 
   strcpy(newstring+newstr_index, string+str_index); 


   return strdup(newstring); 
} 


bool checkException(JNIEnv *env) {
	if (env->ExceptionCheck()==0) return false;
	
	jthrowable exc = env->ExceptionOccurred();
	if (exc==NULL) return false;
	
	//this clears the exception, but we want it to be trown in java again.
	//this will happen when the native method returns.

	//but if we called a java method, the exception remains pending in the 
	//thread until someone clears it (either a java native method returns or we do it!)
	env->ExceptionDescribe();
	env->ExceptionClear();


	//get the stack trace
	jclass c = env->FindClass("jvst/wrapper/system/ExceptionUtililities");
	if (c!=NULL) {
		jmethodID mid = env->GetStaticMethodID(c, "getStackTrace", "(Ljava/lang/Throwable;)Ljava/lang/String;");
		
		if (mid!=NULL) {
			jstring ret = (jstring)env->CallStaticObjectMethod(c, mid, exc);
			if (ret==NULL) {
				log("** ERROR: getStackTrace returned NULL --> aborting!");
				return true;
			}
			const char* jstr = env->GetStringUTFChars(ret, NULL);
			char buffer[10000]; //stack trace could be very big!
			sprintf(buffer, "** %s", jstr); //add the error indicator ('**')
			log(buffer);
		}
		else {
			log("** Error: cannot find MethodID getStackTrace((Ljava/lang/Throwable;)Ljava/lang/String;)");
			env->ExceptionClear();
			env->Throw(exc); //because we want this error to be reported when its possible.
		}
	}
	else {
		log("** Error: Cannot find class jvst/wrapper/system/ExceptionUtililities");
		env->ExceptionClear();
		env->Throw(exc); //because we want this error to be reported when its possible.
	}

	//since we had an exception, return true!
	return true;
}



//check for exception, print stack trace, and throw it again, 
//so it can be catched in java code!
bool checkAndThrowException(JNIEnv *env) {
	jthrowable exc = env->ExceptionOccurred();
	if (exc==NULL) return false;

	bool ret = checkException(env);

	env->Throw(exc);

	return ret;
}









//----------------------------------------------------------
//registry + dynamic lib loading stuff for windows...

#ifndef MACX

#define JVM_REG_18 "Software\\JavaSoft\\Java Runtime Environment\\1.8"
#define JVM_REG_17 "Software\\JavaSoft\\Java Runtime Environment\\1.7"
#define JVM_REG_16 "Software\\JavaSoft\\Java Runtime Environment\\1.6"
#define JVM_REG_15 "Software\\JavaSoft\\Java Runtime Environment\\1.5"
#define JVM_REG_14 "Software\\JavaSoft\\Java Runtime Environment\\1.4"
#define JVM_REG_13 "Software\\JavaSoft\\Java Runtime Environment\\1.3"
#define JVM_REG_12 "Software\\JavaSoft\\Java Runtime Environment\\1.2"


char* readJVMLibLocation(char* requestedJVMVersion) {
	DWORD	rc; 
	HKEY	regKey;
	DWORD	len; 
	DWORD	dwType; 
	char	javaLibLocation[512]; //value stored here
	
	if(requestedJVMVersion==NULL) {
		//Auto check for installed JVMs
		
		//check for jvm 1.8 (future) in registry
		rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, JVM_REG_18, 0, KEY_ALL_ACCESS, &regKey); 
		if (rc != ERROR_SUCCESS) {
			
			//check for jvm 1.7 (future) in registry
			rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, JVM_REG_17, 0, KEY_ALL_ACCESS, &regKey); 
			if (rc != ERROR_SUCCESS) {

				//check for jvm 1.6 (future) in registry
				rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, JVM_REG_16, 0, KEY_ALL_ACCESS, &regKey); 
				if (rc != ERROR_SUCCESS) {
					
					//no 1.6, try 1.5
					rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, JVM_REG_15, 0, KEY_ALL_ACCESS, &regKey); 
					if (rc != ERROR_SUCCESS) {
						
						//no 1.5 try 1.4!
						rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, JVM_REG_14, 0, KEY_ALL_ACCESS, &regKey); 
						if (rc != ERROR_SUCCESS) {
						
							//no 1.4 try 1.3!
							rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, JVM_REG_13, 0, KEY_ALL_ACCESS, &regKey); 
							if (rc != ERROR_SUCCESS) {
							
								//no 1.3 try 1.2!
								rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, JVM_REG_12, 0, KEY_ALL_ACCESS, &regKey); 
								if (rc != ERROR_SUCCESS) {
									return NULL;
								}
							}
						}
					} 
				}
			}
		}
	}
	else {
		//there is a specific jvm version mentioned in the .ini, try to locate this one...
		char jvmRegKey[512] = {'\0'};
		strcat(jvmRegKey, "Software\\JavaSoft\\Java Runtime Environment\\\0");
		strcat(jvmRegKey, requestedJVMVersion);
		
		log("Trying to locate specific jvm version with regkey:");
		log(jvmRegKey);
		
		//check if this key is available
		rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, jvmRegKey, 0, KEY_ALL_ACCESS, &regKey); 
		if (rc != ERROR_SUCCESS) {
			return NULL;
		}
	}

	len = sizeof(javaLibLocation);

	rc = RegQueryValueEx(regKey, "RuntimeLib", 0, &dwType, (unsigned char*)javaLibLocation, &len); 

	if (rc==ERROR_SUCCESS) return strdup(javaLibLocation);
	else return NULL;
}



//globals
jint (JNICALL *PTR_CreateJavaVM)(JavaVM **, void **, void *) = NULL; 
jint (JNICALL *PTR_GetCreatedJavaVMs)(JavaVM **, jsize, jsize *) = NULL;


int initJVMFunctionPointers(char *vmlibpath) {
	//load the library
	HINSTANCE hVM = LoadLibrary(vmlibpath);
    if (hVM == NULL) {
		log("**ERROR: Could not locate jvm.dll. \n\
			It seems that you dont have a properly installed Java Virtual Machine (JVM).\n\
			If so, please download (its free!) and install one from \n\n\
			http://www.java.com/ \n\n\
			If you already did that and see this message again, please add the path \n\
			where the jvm.dll is located to your PATH environment variable.\n\n\
			Per default, the jvm.dll is located in 'C:\\j2sdk1.4.2_05\\jre\\bin\\client'. But this \n\
			may differ depending on your choice during installation. \n\
			You can modify your PATH variable if you click \n\n\
			Start/Settings/Control Panel/System/Advanced (Tab)/Environment Variables (Button)\n\
			/System Variables (the textbox at the bottom).\n\
			Double-click on the variable called Path, and add a ';the-path-where-jvm.dll-is-located' \n\
			to the end of its value.\n\
			Note the ';' at the beginning of the string. close all windows with ok, and you \n\
			should be all set.\n\
			If you still cant use the plugin, contact me at \n\ndaniel309@users.sourgeforge.net"); 

		return -1;
	}
	
	PTR_CreateJavaVM = (jint (JNICALL *)(JavaVM **, void **, void *))GetProcAddress(hVM, "JNI_CreateJavaVM");
    PTR_GetCreatedJavaVMs = (jint (JNICALL *)(JavaVM **, jsize, jsize *))GetProcAddress(hVM, "JNI_GetCreatedJavaVMs");
	if (PTR_CreateJavaVM==NULL || PTR_GetCreatedJavaVMs==NULL) {
		log("**ERROR: Cant find jvm interface pointers!");
		return -1;
	}
	
	return 0;
}

#else
//**************************************************************************************************
// Mac feature for requesting a specific JVM


/*
To invoke Java 1.4.1 or the currently preferred JDK as defined by the operating system 
(1.4.2 as of the release of this sample and the release of Mac OS X 10.4) nothing changes in 10.4 vs 10.3 
in that when a JNI_VERSION_1_4 is passed into JNI_CreateJavaVM as the vm_args.version it returns 
the current preferred JDK.

To specify the current preferred JDK in a family of JVM's, say the 1.5.x family, applications should set 
the environment variable JAVA_JVM_VERSION to 1.5, and then pass JNI_VERSION_1_4 into JNI_CreateJavaVM 
as the vm_args.version. To get a specific Java 1.5 JVM, say Java 1.5.0, set the environment variable 
JAVA_JVM_VERSION to 1.5.0. For Java 1.6 it will be the same in that applications will need to set 
the environment variable JAVA_JVM_VERSION to 1.6 to specify the current preferred 1.6 Java VM, and 
to get a specific Java 1.6 JVM, say Java 1.6.1, set the environment variable JAVA_JVM_VERSION to 1.6.1.

To make this sample bring up the current preferred 1.5 JVM, set the environment variable 
JAVA_JVM_VERSION to 1.5 before calling JNI_CreateJavaVM as shown below.  Applications must currently 
check for availability of JDK 1.5 before requesting it.  If your application requires JDK 1.5 and it is not 
found, it is your responsibility to report an error to the user. To verify if a JVM is installed, check 
to see if the symlink, or directory exists for the JVM in /System/Library/Frameworks/JavaVM.framework/Versions/ 
before setting the environment variable JAVA_JVM_VERSION.

If the environment variable JAVA_JVM_VERSION is not set, and JNI_VERSION_1_4 is passed into 
JNI_CreateJavaVM as the vm_args.version, JNI_CreateJavaVM will return the current preferred JDK. 
Java 1.4.2 is the preferred JDK as of the release of this sample and the release of Mac OS X 10.4.
*/


int checkJVMVersionRequest(char* requestedJVMVersion) {
	if (requestedJVMVersion==NULL) return -1;
	
	log("checking for a jvm version");
	log(requestedJVMVersion);
	
	CFStringRef targetJVM = CFStringCreateWithCString(NULL, requestedJVMVersion, kCFStringEncodingASCII);
	CFBundleRef JavaVMBundle;
	CFURLRef    JavaVMBundleURL;
	CFURLRef    JavaVMBundlerVersionsDirURL;
	CFURLRef    TargetJavaVM;
	UInt8 pathToTargetJVM [PATH_MAX] = "\0";
	struct stat sbuf;
	int retval = -1;
	
	// Look for the JavaVM bundle using its identifier
	JavaVMBundle = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.JavaVM") );
	
	if(JavaVMBundle != NULL) {
		// Get a path for the JavaVM bundle
		JavaVMBundleURL = CFBundleCopyBundleURL(JavaVMBundle);
		CFRelease(JavaVMBundle);
		
		if(JavaVMBundleURL != NULL) {
			// Append to the path the Versions Component
			JavaVMBundlerVersionsDirURL = CFURLCreateCopyAppendingPathComponent(kCFAllocatorDefault,JavaVMBundleURL,CFSTR("Versions"),true);
			CFRelease(JavaVMBundleURL);
			
			if(JavaVMBundlerVersionsDirURL != NULL) {
				// Append to the path the target JVM's Version
				TargetJavaVM = CFURLCreateCopyAppendingPathComponent(kCFAllocatorDefault,JavaVMBundlerVersionsDirURL,targetJVM,true);
				CFRelease(JavaVMBundlerVersionsDirURL);
				
				if(TargetJavaVM != NULL) {
					if(CFURLGetFileSystemRepresentation (TargetJavaVM,true,pathToTargetJVM,PATH_MAX )) {
						// Check to see if the directory, or a sym link for the target JVM directory exists, and if so set the
						// environment variable JAVA_JVM_VERSION to the target JVM.
						if(stat((char*)pathToTargetJVM,&sbuf) == 0) {
							// Ok, the directory exists, so now we need to set the environment var JAVA_JVM_VERSION to the CFSTR targetJVM
							// We can reuse the pathToTargetJVM buffer to set the environement var.
							if(CFStringGetCString(targetJVM, (char*)pathToTargetJVM, PATH_MAX, kCFStringEncodingUTF8)) {
								setenv("JAVA_JVM_VERSION", (char*)pathToTargetJVM, 1);
								retval = 0;
								log("Found a JVM that matches the one requested, loading that one!!!");
								log((char*)pathToTargetJVM);
							} else log("Error getting path to target jvm");
						} else log("Error checking symlink for target jvm");
					} else log("Error getting file system representation for bundle url");
					CFRelease(TargetJavaVM);
				} else log("Error appending version component to bundle url");
			} else log("Error appending path component to bundle url");
		} else log("Error copying bulde url");
	} else log("ERROR: cant find bundle: com.apple.JavaVM");
	
	
	return retval;
}

#endif