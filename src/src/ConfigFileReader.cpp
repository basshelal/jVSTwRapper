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
// INI file Reader
//-
// 2003,2004 Daniel Martin
//-------------------------------------------------------------------------------------------------------


#include "ConfigFileReader.h"
#include "JNIUtils.h"

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
		log("** Error opening config file at ");
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
	this->AttachToNativePluginWindow = 1;
	this->CloseNativePluginWindow = 0;
	this->CustomJVMLocation = NULL;
	this->RequestedJVMVersion = NULL;

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
			log("** INI File Read error");
			break;
		}

		if(line==NULL) break;
		line = trim(line);

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
		if(strcmp(token, "CustomJVMLocation")==0) {
			token = strtok(NULL, "\n");
			if (token != NULL) this->CustomJVMLocation = strdup(token);
		}
		if(strcmp(token, "RequestJVMVersion")==0) {
			token = strtok(NULL, "\n");
			if (token != NULL) this->RequestedJVMVersion = strdup(token);
		}
		if(strcmp(token, "SystemClassPath")==0) {
			token = strtok(NULL, sep);
			if (token != NULL) this->SystemClassPath = strdup(token);
			else log("**ERROR: finding SystemClassPath token");
		}
		if(strcmp(token, "IsLoggingEnabled")==0) {
			token = strtok(NULL, sep);
			if (token != NULL) {
				char* tmp = trim(strdup(token));
				this->IsLoggingEnabled = atoi(tmp);
			}
			else log("**ERROR: finding IsLoggingEnabled token");
		}
		if(strcmp(token, "CloseNativePluginWindow")==0) {
			token = strtok(NULL, sep);
			if (token != NULL) {
				char* tmp = trim(strdup(token));
				this->CloseNativePluginWindow = atoi(tmp);
			}
			else log("**ERROR: finding CloseNativePluginWindow token");
		}
		if(strcmp(token, "AttachToNativePluginWindow")==0) {
			token = strtok(NULL, sep);
			if (token != NULL) {
				char* tmp = trim(strdup(token));
				this->AttachToNativePluginWindow = atoi(tmp);
			}
			else log("**ERROR: finding AttachToNativePluginWindow token");
		}
	}

	if (this->JVMClassPath==NULL || this->PluginClass==NULL || this->SystemClassPath==NULL) 
		log("**ERROR: not enough config values found (ClassPath, SystemClassPath and PluginClass are mandatory)!");
}

