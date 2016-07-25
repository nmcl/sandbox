/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * Stack interface definition. (Basic)
 */

enum stackOutcome { unknown, stack_full, stack_empty, not_done, done };

const int stackSize = 32;

class Stack
{
public:
    Stack(enum stackOutcome& outcome);
    ~Stack();

    void Push(enum stackOutcome& outcome, int  value);
    void Pop (enum stackOutcome& outcome, int& value);
private:
    int values[stackSize];
    int top;
};
