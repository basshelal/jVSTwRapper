//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// INI File Reader
//-
// 2003,2004 Daniel Martin
//-------------------------------------------------------------------------------------------------------

#ifndef __ConfigFileReader__
#define __ConfigFileReader__

#include <stdio.h>


class ConfigFileReader {

	public:
		ConfigFileReader ();
		virtual ~ConfigFileReader ();

		//INI File Entries
		char* PluginClass;
		char* PluginUIClass;
		char* JVMClassPath;
		char* JVMOption1;
		char* JVMOption2;
		char* JVMOption3;
		char* JVMOption4;
		char* JVMOption5;
		char* SystemClassPath;
		int IsLoggingEnabled;
		int CloseNativePluginWindow;
		int AttachToNativePluginWindow;


	protected:
		virtual void ReadConfigFile ();

	private:
		FILE* INIFile;

};

#endif
