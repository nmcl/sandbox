/*
 * Copyright (C) 1997,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

#include <BufferCheck.h>
#include <DummyBuffer.h>

class Combo : public DummyBuffer, public BufferCheck
{
public:
    Combo (int&);
    ~Combo ();

    void Test (Buffer, Buffer&);
    void foobar ();    
};
