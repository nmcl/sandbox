/*
 * Copyright (C) 1997,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Stack.h,v 1.1 1997/09/25 15:32:07 nmcl Exp $
 */

#ifndef STACK_H_
#define STACK_H_

static const int STACK_SIZE = 10;

#include <Common/Boolean.h>

class Stack
{
public:
    Stack (Boolean&);
    virtual ~Stack ();

    Boolean push (int);
    Boolean pop (int&);
    
private:
    int theStack[STACK_SIZE];
    int stackPointer;
};

#endif
