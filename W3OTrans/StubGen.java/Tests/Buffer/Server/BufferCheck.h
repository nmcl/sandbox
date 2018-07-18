/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BufferCheck.h,v 1.1 1994/07/01 10:53:43 ngdp Exp $
 */

#ifndef BUFFERCHECK_H_
#define BUFFERCHECK_H_

class Bank
{
public:
    Bank ();
    Bank (int&);
    ~Bank ();

    int insert (int pin, int amount);
    int withdraw (int pin, double& amount);
    int inspect (int pin, int* amount);
    int transfer (int pinFrom, int pinTo, int amount);

private:
    int theBank[10];
};

#endif
