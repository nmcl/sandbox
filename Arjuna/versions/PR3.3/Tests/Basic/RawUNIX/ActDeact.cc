/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActDeact.cc,v 1.6 1994/12/06 09:27:17 ngdp Exp $
 */

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream.h>
#include <stdlib.h>
#include <memory.h>

#include <ElapsedTime.h>

void doTest(int bufSize)
{
    ElapsedTime totalDeact;
    ElapsedTime startPart, endPart;

    char fname[64];
    int flags = O_RDWR | O_CREAT;
    char *buffer    = new char[bufSize];
    char *dataBlock = new char[bufSize];

    int i = 0;

    sprintf(fname, "perftest/%d/%d.dat", bufSize, i);

    int fd = ::open("fred", flags, 0666);

    if (fd == -1)
    {
	perror("Activate: Open failed");
        exit(1);
    }

    startPart.get();
    for (i = 0; i < 1000; i++)
    {
	    if (lseek(fd, 0, SEEK_SET))
	    {
	        perror("Deactivate: lseek failed");
	        exit(1);
	    }

	    if (write(fd, dataBlock, bufSize) != bufSize)
	    {
	        perror("Deactivate: write failed");
	        exit(1);
	    }
    }
    endPart.get();
    totalDeact = endPart - startPart;

    close(fd);

    cout << "Deactivate time: " << totalDeact << endl;

    delete dataBlock;
    delete buffer;
}

int main(int, char *[])
{
    cout << endl << "Test 3 - 1k byte object" << endl;
    doTest(1024);
    return 0;
}

