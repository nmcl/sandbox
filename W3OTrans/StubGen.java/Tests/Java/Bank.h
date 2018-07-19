/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

#ifndef BANK_H_
#define BANK_H_

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
