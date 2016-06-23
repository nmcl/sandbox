/*
 * Copyright (C) 1997,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Stack.cc,v 1.1 1997/09/25 15:32:04 nmcl Exp $
 */

#define STACK_SERVER

#ifndef STACK_H_
#  include "Stack_stub.h"
#endif

Stack::Stack (Boolean& res)
{
    for (int i = 0; i < STACK_SIZE; i++)
	theStack[i] = 0;

    stackPointer = -1;
    
    res = TRUE;
}

Stack::~Stack ()
{
}

Boolean Stack::push (int val)
{
    if (stackPointer == STACK_SIZE-1)
	return FALSE;

    stackPointer++;
    theStack[stackPointer] = val;

    return TRUE;
}

Boolean Stack::pop (int& val)
{
    if (stackPointer == -1)
	return FALSE;

    val = theStack[stackPointer];
    stackPointer--;

    return TRUE;
}
