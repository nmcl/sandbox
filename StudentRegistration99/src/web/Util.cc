/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Util.cc,v 1.4 1996/08/29 12:34:15 arjuna Exp $
 */

#include <System/malloc.h>
#include <System/memory.h>
#include <System/string.h>
#include <System/stdio.h>
#include <System/sys/types.h>
#include <System/unistd.h>
#include <System/fcntl.h>
#include <System/sys/stat.h>
#include <System/errno.h>

#include <Common/Boolean.h>

#ifndef COMMON_H_
#  include <Common.h>
#endif

#define LF 10
#define CR 13

Boolean checkScript (const char* root, const char* name)
{
    char* fullName = ::new char[::strlen(root)+::strlen(name)+1];
    ::strcpy(fullName, root);
    ::strcat(fullName, name);

    struct stat buf;

    if (stat(fullName, &buf) == 0)
    {
	::delete [] fullName;
	
	return TRUE;
    }
    else
    {
	printf("<BODY BGCOLOR=#FFFFFF>");
	
	if (errno == EACCES)
	    printf("<FONT COLOR=#FF0000>Access to script denied for: %s\n", fullName);
	else
	{
	    if ((errno == EFAULT) || (errno == ENOENT))
		printf("<FONT COLOR=#FF0000>Could not find the script: %s\n", fullName);
	    else
		printf("<FONT COLOR=#FF0000>An error has occurred trying to check script: %s\n", fullName);
	}

	printf("</FONT></BODY>");
	
	::delete [] fullName;
	
	return FALSE;
    }
}

void printerror (size_t size, const char* error)
{
    int fd = open(error, O_RDONLY);

    char* errormsg = new char[size+1];
    ::memset(errormsg, '\0', size+1);

    read(fd, (void*) errormsg, size);
    
    printf("<FONT COLOR=#FF0000>An error has occurred. The error output is:<PRE>\n%s", errormsg);
    printf("</PRE></FONT>");

    delete errormsg;

    close(fd);
}

void printoutput (size_t size, const char* output)
{
    int fd = open(output, O_RDONLY);

    char* outputmsg = new char[size+1];
    ::memset(outputmsg, '\0', size+1);

    read(fd, (void*) outputmsg, size);

    printf("<PRE>");
    printf("%s", outputmsg);
    printf("</PRE>");

    delete outputmsg;

    close(fd);
}

void getword(char *word, char *line, char stop)
{
    int x = 0,y;

    for(x=0;((line[x]) && (line[x] != stop));x++)
        word[x] = line[x];

    word[x] = '\0';
    if(line[x]) ++x;
    y=0;

    while(line[y++] = line[x++]);
}

char *makeword(char *line, char stop)
{
    int x = 0,y;
    char *word = (char *) malloc(sizeof(char) * (strlen(line) + 1));

    for(x=0;((line[x]) && (line[x] != stop));x++)
        word[x] = line[x];

    word[x] = '\0';
    if(line[x]) ++x;
    y=0;

    while(line[y++] = line[x++]);
    return word;
}

char *fmakeword(FILE *f, char stop, int *cl)
{
    int wsize;
    char *word;
    int ll;

    wsize = 102400;
    ll=0;
    word = (char *) malloc(sizeof(char) * (wsize + 1));

    while(1)
    {
        word[ll] = (char)fgetc(f);
        if(ll==wsize)
	{
            word[ll+1] = '\0';
            wsize+=102400;
            word = (char *)realloc(word,sizeof(char)*(wsize+1));
        }
        --(*cl);
        if((word[ll] == stop) || (feof(f)) || (!(*cl)))
	{
            if(word[ll] != stop) ll++;
            word[ll] = '\0';
            return word;
        }
        ++ll;
    }

    return 0;  // never get here, but ...
}

char x2c(char *what)
{
    register char digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
    digit *= 16;
    digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));
    return(digit);
}

void unescape_url(char *url)
{
    register int x,y;

    for(x=0,y=0;url[y];++x,++y)
    {
        if((url[x] = url[y]) == '%')
	{
            url[x] = x2c(&url[y+1]);
            y+=2;
        }
    }
    url[x] = '\0';
}

void plustospace(char *str)
{
    register int x;

    for(x=0;str[x];x++) if(str[x] == '+') str[x] = ' ';
}

int rind(char *s, char c)
{
    register int x;
    for(x=strlen(s) - 1;x != -1; x--)
        if(s[x] == c) return x;
    return -1;
}

int getline(char *s, int n, FILE *f)
{
    register int i=0;

    while(1)
    {
        s[i] = (char)fgetc(f);

        if(s[i] == CR)
            s[i] = fgetc(f);

        if((s[i] == 0x4) || (s[i] == LF) || (i == (n-1)))
	{
            s[i] = '\0';
            return (feof(f) ? 1 : 0);
        }
        ++i;
    }

    return 0;  // never get here, but ...
}

void send_fd(FILE *f, FILE *fd)
{
    int num_chars=0;
    char c;

    while (1)
    {
        c = fgetc(f);
        if(feof(f))
            return;
        fputc(c,fd);
    }
}

int ind(char *s, char c)
{
    register int x;

    for(x=0;s[x];x++)
        if(s[x] == c) return x;

    return -1;
}

void escape_shell_cmd(char *cmd)
{
    register int x,y,l;

    l=strlen(cmd);
    for(x=0;cmd[x];x++)
    {
        if(ind("&;`'\"|*?~<>^()[]{}$\\",cmd[x]) != -1)
	{
            for(y=l+1;y>x;y--)
                cmd[y] = cmd[y-1];
            l++; /* length has been increased */
            cmd[x] = '\\';
            x++; /* skip the character */
        }
    }
}

