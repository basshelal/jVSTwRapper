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


//hi gerard! 
//I commented this out that you get some compiler errors ;-)
//maybe you want to implement the message box feature for the mac too?

//#ifndef MACX
	//show MessageBox on Error or warning!
	//restrict it to 5 messages per session.
	//more would just be annoying...
	if (isWarningOrError) {
		MessageBoxCount++;	
		if (MessageBoxCount<5) MessageBoxA(0, data, "jVSTwRapper", 0);
	}
//#endif

	if (IsLogEnabled || isWarningOrError) {
		fprintf(stderr, "\n");
		fprintf(stderr, data);
		fflush(stderr);
		retval = 0;
	} else retval = 0;

    return retval;
}


char *replace(char *string, char *oldpiece, char *newpiece) { 
   int str_index, newstr_index, oldpiece_index, end, 
      new_len, old_len, cpy_len; 
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
			const char* jstr = env->GetStringUTFChars(ret, NULL);
			char buffer[10000]; //stack trace can be very big!
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

#define JVM_REG_16 "Software\\JavaSoft\\Java Runtime Environment\\1.6"
#define JVM_REG_15 "Software\\JavaSoft\\Java Runtime Environment\\1.5"
#define JVM_REG_14 "Software\\JavaSoft\\Java Runtime Environment\\1.4"
#define JVM_REG_13 "Software\\JavaSoft\\Java Runtime Environment\\1.3"
#define JVM_REG_12 "Software\\JavaSoft\\Java Runtime Environment\\1.2"


char* readJVMLibLocation() {
	DWORD	rc; 
	HKEY	regKey;
	DWORD	len; 
	DWORD	dwType; 
	char	javaLibLocation[512]; //value stored here
	

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



#endif