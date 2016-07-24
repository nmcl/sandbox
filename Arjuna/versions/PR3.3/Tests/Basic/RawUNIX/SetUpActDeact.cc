/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SetUpActDeact.cc,v 1.3 1994/10/28 12:39:15 ngdp Exp $
 */

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream.h>
#include <stdlib.h>
#include <memory.h>

void testSetup(int fileSize)
{
    char fname[64];
    int flags = O_RDWR | O_CREAT;

    mkdir("perftest", 0755);

    sprintf(fname, "perftest/%d", fileSize);

    mkdir(fname, 0755);

    for (int i = 0; i < 100; i++)
    {   
        sprintf(fname, "perftest/%d/%d.dat", fileSize, i);













        ::unlink(fname);
    
        int fd = ::open(fname, flags, 0666);
        char *buffer = (char*) malloc(fileSize);

        if (fd == -1)
        {
    	    perror("setupTest: Open failed");
	    exit(1);
        }
        else
        {
            if (write(fd, buffer, fileSize) != fileSize)
            {
	        perror("setupTest: Write failed");
	        exit(1);
            }
	
	    close(fd);
        }
    }
}  

int main(int, char *[])
{
    cout << endl << "SetUp 3 - 1k byte object" << endl;
    testSetup(1024);
    return 0;
    
}
