/*
 * Copyright (C) 1994
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: Forker.cc,v 1.3 1995/02/13 11:31:58 ngdp Exp $
 */

#include <System/stdlib.h>
#include <System/stdio.h>
#include <System/unistd.h>
#include <System/sys/wait.h>
#include <System/string.h>
#include <System/iostream.h>

#include <Common/Boolean.h>

#include "ElapsedTime.h"
#include "CpuTime.h"

int main(int, char *[])
{
    ElapsedTime startTime, finishTime, resultTime;
    CpuTime startCpu, finishCpu, overallCpu;

    startTime.get(); startCpu.get();

    for (int i = 0; i < 1000; i++)
    {
         pid_t pid;
#ifdef HAVE_UNION_WAIT
         union wait w;
#else
         int w; 
#endif
         pid = fork();

         if (pid == 0)
         {
             pid = fork();

             if (pid == 0)
             {
                 if (execve("./Null1", NULL, NULL))
                 {
                     perror("execve");
                     exit(1);
                 }
             }
             else if (pid == -1)
             {
                 perror("fork");
                 exit(1);
             }
             else
             {
                 wait(&w);
                 exit(0);
             }
         }
         else if (pid == -1)
         {
             perror("fork");
             exit(1);
         }
         else
             wait(&w);
    }
    
    finishTime.get(); finishCpu.get();

    resultTime = finishTime - startTime;
    overallCpu = finishCpu - startCpu;

    cout <<"--------------------------------------------" << endl;
    cout << "Overall Time     : " <<  resultTime << endl;
    cout << "Overall Cpu Time : " <<  overallCpu << endl;    
    cout << "-------------------------------------------" << endl;

    return 0;
}
