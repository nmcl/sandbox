/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * Stack implementation. (Distributed)
 */

#define STACK_SERVER
#include "Stack_stub.h"

Stack::Stack(enum stackOutcome& outcome)
{
    outcome = done;
    top = 0;
}

Stack::~Stack()
{
}

void Stack::Push(enum stackOutcome& outcome, int  value)
{
    outcome = unknown;

    if (top < stackSize)
    {
	values[top] = value;
	top ++;
	outcome = done;
    }
    else
	outcome = stack_full;
}

void Stack::Pop (enum stackOutcome& outcome, int& value)
{
    outcome = unknown;

    if (top > 0)
    {
	top --;
	value = values[top];
	outcome = done;
    }
    else
	outcome = stack_empty;
}
