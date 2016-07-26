/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: regsys.cc,v 1.4 1996/08/29 12:34:19 arjuna Exp $
 */

#include <System/string.h>
#include <System/stdio.h>
#include <System/stdlib.h>
#include <System/iostream.h>
#include <System/fstream.h>
#include <System/sys/types.h>
#include <System/sys/stat.h>
#include <System/dirent.h>
#include <System/sys/time.h>
#include <System/unistd.h>

#include <Common/Boolean.h>

#ifndef COMMON_H_
#  include <Common.h>
#endif

#include <local.h>

const char* start = "startup";
const char* ShutDown = "shutdown";
const char* pauseDrivers = "Pause_drivers";
const char* purge = "purgeLogs";
const char* cleanup = "cleanup";
const char* addDump = "AddDumping";
const char* removeDump = "RemoveDumping";
const char* inspectDrivers = "InspectWorkers";
const char* objResources = "ObjStoreResourcesCheck";
const char* objProcesses = "ObjStoreProcessCheck";
const char* objLogs = "Logs/ObjStoreLogsCheck";
const char* driverResources = "WorkResourcesCheck";
const char* driverProcesses = "WorkProcessCheck";
const char* driverLogs = "Logs/WorkLogsCheck";
const char* objLoad = "ObjStoreLoadCheck";
const char* driverLoad = "WorkLoadCheck";

int parse (const char* s)
{
    const char* script = (char*) 0;
    int opcode = atoi(s);
    
    switch (opcode)
    {
    case 1:
	script = start;
	break;
    case 2:
	script = ShutDown;
	break;
    case 3:
	script = pauseDrivers;
	break;
    case 4:
	script = purge;
	break;
    case 5:
	script = cleanup;
	break;
    case 6:
	script = addDump;
	break;
    case 7:
	script = removeDump;
	break;
    case 8:
	script = driverResources;
	break;
    case 9:
	script = inspectDrivers;
	break;
    case 10:
	script = objResources;
	break;
    case 11:
	script = objProcesses;
	break;
    case 12:
	script = objLogs;
	break;
    case 13:
	script = driverProcesses;
	break;
    case 14:
	script = driverLogs;
	break;
    case 15:
	script = objLoad;
	break;
    case 16:
	script = driverLoad;
	break;
    }

    if (!script)
	return -1;

    if (!checkScript(scriptRoot, script))
	return 0;
    
    pid_t pid = getpid();
    char output[MAX_FILE_NAME], error[MAX_FILE_NAME];
    memset(output, '\0', MAX_FILE_NAME);
    memset(error, '\0', MAX_FILE_NAME);

    sprintf(output, outputFile, pid);
    sprintf(error, errorFile, pid);
	
    char* command = new char[strlen(scriptRoot)+strlen(script)+strlen(output)+strlen(error)+3];
    strcpy(command, scriptRoot);
    strcat(command, script);
    strcat(command, " ");
    strcat(command, output);
    strcat(command, " ");
    strcat(command, error);

    system(command);

    ::delete command;
	
    struct stat buf;
    Boolean done = FALSE;

    if (stat(error, &buf) == 0)
    {
	if (buf.st_size > 0)
	{
	    printf("<BODY BGCOLOR=#FFFFFF>");
	    printerror((size_t) buf.st_size, error);
	    printf("</BODY>");
	}
	else
	    done = TRUE;
    }
    else
	done = TRUE;
    
    if (done)
    {
	stat(output, &buf);

	printf("<BODY BGCOLOR=#FFFFFF>");    

	if (buf.st_size > 0)
	    printoutput((size_t) buf.st_size, output);
	else
	    printf("No output produced.\n");

	printf("</BODY>");
    }

    unlink(error);
    unlink(output);
    
    return 0;
}

int main (int, char **)
{
    int result = 0;
    char* cl = (char*) 0;

    printf("Content-type: text/html%c%c",10,10);

    if(strcmp(getenv("REQUEST_METHOD"),"GET"))
    {
        printf("This script should be referenced with a METHOD of GET.\n");
        exit(1);
    }
    
    cl = getenv("QUERY_STRING");

    printf("<HTML>");
    
    fflush(stdout);

    if (!cl)
	result = -2;
    else
	result = parse(cl);
    
    if (result != 0)
    {
	printf("<BODY BGCOLOR=#FFFFFF>");
	printf("<FONT COLOR=#FF0000>Error number<B> %d </B>occurred.", result);

	if (result == -2)
	    printf("<P>No query string was found.%c", 10);
	else
	    printf("%c", 10);

	printf("</FONT></BODY></HTML>");
	exit(1);
    }

    printf("</HTML>");
    
    return 0;
}
