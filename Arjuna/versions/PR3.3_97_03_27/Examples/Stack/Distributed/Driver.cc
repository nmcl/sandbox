/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 *  Stack drived program. (Distributed)
 */

#include <Config/Configure.h>

#include <iostream.h>

#include "Stack_stub.h"

void Message(const char* text, const enum stackOutcome outcome)
{
    cout << text << " : ";
    switch (outcome)
    {
        case unknown:
	    cout << "unknown\n" << flush;
	    break;
	case stack_full:
	    cout << "stack full\n" << flush;
	    break;
	case stack_empty:
	    cout << "stack empty\n" << flush;
	    break;
	case not_done:
	    cout << "not done\n" << flush;
	    break;
	case done:
	    cout << "done\n" << flush;
	    break;
	default:
	    cout << "** unknown **\n" << flush;
	    break;
    }
}

int main(int argc, char** argv)
{
    int               value;
    enum stackOutcome outcome;

    outcome = unknown;
    Stack stack(outcome);
    Message("Constructor for Stack object", outcome);

    outcome = unknown;
    value = 0;
    stack.Pop(outcome, value);
    Message("Pop", outcome);
    cout << "Value returned is " << value << "\n" << flush;

    outcome = unknown;
    value = 12;
    stack.Push(outcome, value);
    Message("Push of 12", outcome);

    outcome = unknown;
    value = 24;
    stack.Push(outcome, value);
    Message("Push of 24", outcome);

    outcome = unknown;
    value = 34;
    stack.Push(outcome, value);
    Message("Push of 34", outcome);

    outcome = unknown;
    value = 0;
    stack.Pop(outcome, value);
    Message("Pop", outcome);
    cout << "Value returned is " << value << "\n" << flush;

    outcome = unknown;
    value = 0;
    stack.Pop(outcome, value);
    Message("Pop", outcome);
    cout << "Value returned is " << value << "\n" << flush;

    return 0;
}
