//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// INI file Reader
//-
// 2003,2004 Daniel Martin
//-------------------------------------------------------------------------------------------------------

#ifndef __ConfigFileReader__
#include "ConfigFileReader.h"
#endif

#ifndef __JNIUtils__
#include "JNIUtils.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>




extern char DllPath[512];
extern char ConfigFileName[100];


ConfigFileReader::ConfigFileReader() {
	char ConfigFileLocation[700];

	strcpy(ConfigFileLocation, DllPath);
	strcat(ConfigFileLocation, ConfigFileName);
	
	//log(ConfigFileLocation);

	this->INIFile = fopen(ConfigFileLocation, "rt");
	if(this->INIFile==NULL) {
		log("Error opening config file at ");
		log(ConfigFileLocation);
	}

	//init config fields
	this->PluginClass = NULL;
	this->PluginUIClass = NULL;
	this->JVMClassPath = NULL;
	this->JVMOption1 = NULL;
	this->JVMOption2 = NULL;
	this->JVMOption3 = NULL;
	this->JVMOption4 = NULL;
	this->JVMOption5 = NULL;
	this->SystemClassPath = NULL;
	this->IsLoggingEnabled = 0;
	this->CloseNativePluginWindow = 1;


	this->ReadConfigFile();

	fclose(this->INIFile);
}

ConfigFileReader::~ConfigFileReader() {
	//nix
}

void ConfigFileReader::ReadConfigFile() {
	char line[1024];
	char sep[3] = "=\n";
	char* token;


	while(!feof(this->INIFile)) {
		fgets(line, 1024, this->INIFile);
		
		if(ferror(this->INIFile)) {
			log("INI File Read error");
			break;
		}

		if(line==NULL) break;

		//tokenize
		token = strtok(line, sep);
		if(token==NULL) continue;

		if(strcmp(token, "PluginClass")==0) {
			token = strtok(NULL, sep);
			if (token != NULL) this->PluginClass = strdup(token);
			else log("**ERROR: finding PluginClass token");
		}
		if(strcmp(token, "PluginUIClass")==0) {
			token = strtok(NULL, sep);
			if (token != NULL) this->PluginUIClass = strdup(token);
			else log("**ERROR: finding PluginUIClass token");
		}
		if(strcmp(token, "ClassPath")==0) {
			token = strtok(NULL, sep);
			if (token != NULL) this->JVMClassPath = strdup(token);
			else log("**ERROR: finding JVMClassPath token");
		}
		if(strcmp(token, "JVMOption1")==0) {
			token = strtok(NULL, "\n");
			if (token != NULL) this->JVMOption1 = strdup(token);
		}
		if(strcmp(token, "JVMOption2")==0) {
			token = strtok(NULL, "\n");
			if (token != NULL) this->JVMOption2 = strdup(token);
		}
		if(strcmp(token, "JVMOption3")==0) {
			token = strtok(NULL, "\n");
			if (token != NULL) this->JVMOption3 = strdup(token);
		}
		if(strcmp(token, "JVMOption4")==0) {
			token = strtok(NULL, "\n");
			if (token != NULL) this->JVMOption4 = strdup(token);
		}
		if(strcmp(token, "JVMOption5")==0) {
			token = strtok(NULL, "\n");
			if (token != NULL) this->JVMOption5 = strdup(token);
		}
		if(strcmp(token, "SystemClassPath")==0) {
			token = strtok(NULL, sep);
			if (token != NULL) this->SystemClassPath = strdup(token);
			else log("**ERROR: finding SystemClassPath token");
		}
		if(strcmp(token, "IsLoggingEnabled")==0) {
			token = strtok(NULL, sep);
			if (token != NULL) {
				char* tmp = strdup(token);
				this->IsLoggingEnabled = atoi(tmp);
			}
			else log("**ERROR: finding IsLoggingEnabled token");
		}
		if(strcmp(token, "CloseNativePluginWindow")==0) {
			token = strtok(NULL, sep);
			if (token != NULL) {
				char* tmp = strdup(token);
				this->CloseNativePluginWindow = atoi(tmp);
			}
			else log("**ERROR: finding CloseNativePluginWindow token");
		}
	}

	if (this->JVMClassPath==NULL || this->PluginClass==NULL || this->SystemClassPath==NULL) 
		log("**ERROR: not enough config values found (ClassPath, SystemClassPath and PluginClass are mandatory)!");
}

