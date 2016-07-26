/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: unlockstudent.cc,v 1.4 1996/08/29 12:34:20 arjuna Exp $
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

const char* shellName = "UnlockStudent ";

int unlock (entry entries[], int number)
{
    char* id = 0;
    char* name = 0;
    
    for (int i = 0; i < number; i++)
    {
	if (::strcmp(entries[i].name, "Student Number") == 0)
	    id = entries[i].val;
	if (::strcmp(entries[i].name, "Machine Name") == 0)
	    name = entries[i].val;	
    }

    if (!id || (strlen(id) == 0))
	return -1;

    if (!name || (strlen(name) == 0))
	return -1;

    pid_t pid = getpid();
    char output[MAX_FILE_NAME], error[MAX_FILE_NAME];
    memset(output, '\0', MAX_FILE_NAME);
    memset(error, '\0', MAX_FILE_NAME);

    sprintf(output, outputFile, pid);
    sprintf(error, errorFile, pid);
    
    char* command = ::new char[::strlen(scriptRoot)+::strlen(shellName)+::strlen(name)+::strlen(id)+strlen(output)+strlen(error)+4];
    strcpy(command, scriptRoot);
    strcat(command, shellName);
    strcat(command, name);
    strcat(command, " ");
    strcat(command, id);
    strcat(command, " ");
    strcat(command, output);
    strcat(command, " ");
    strcat(command, error);

    system(command);

    ::delete command;

    struct stat buf;
    Boolean done = FALSE;

    /*
     * If no error or output files, the assume ok.
     */
    
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
    entry entries[MAX_ENTRIES];
    register int x, m = 0;
    int cl, result = 0;

    printf("Content-type: text/html%c%c",10,10);

    if(strcmp(getenv("REQUEST_METHOD"),"POST"))
    {
        printf("This script should be referenced with a METHOD of POST.\n");
        exit(1);
    }
    if(strcmp(getenv("CONTENT_TYPE"),"application/x-www-form-urlencoded"))
    {
        printf("This script can only be used to decode form results.\n");
        exit(1);
    }

    cl = atoi(getenv("CONTENT_LENGTH"));

    for(x = 0;cl && (!feof(stdin));x++)
    {
	m = x;
	entries[x].val = fmakeword(stdin,'&',&cl);
	plustospace(entries[x].val);
	unescape_url(entries[x].val);
	entries[x].name = makeword(entries[x].val,'=');
    }

    printf("<HTML>");

    fflush(stdout);
    
    if (checkScript(scriptRoot, shellName))
    {
	result = unlock(entries, m+1);
    
	if (result != 0)
	{
	    printf("<BODY BGCOLOR=#FFFFFF>");
	    printf("<FONT COLOR=#FF0000>Error number<B> %d </B>occurred when attempting to unlock student.", result);

	    if (result == -1)
		printf("<P>You must give a valid student number and machine name.%c", 10);
	    else
		printf("%c", 10);

	    printf("</FONT></BODY></HTML>");
	    exit(1);
	}
    }

    printf("</HTML>");

    return 0;
}
