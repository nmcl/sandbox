/*
 * Copyright (C) 1994
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: Null1.cc,v 1.3 1994/09/23 08:39:25 nsmw Exp $
 */

#include <System/stdio.h>
#include <System/unistd.h>
#include <System/stdlib.h>

char data[1000000]  = { 1 };

int main(int, char *[])
{
    if (execve("./Null2", NULL, NULL))
    {
        perror("execve");
        exit(1);
    }

     return 0;
}
