/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BufferCheck.h,v 1.1 1997/06/09 19:52:28 nmcl Exp $
 */

#ifndef BUFFERCHECK_H_
#define BUFFERCHECK_H_

#include <Common/Buffer.h>

class A
{
public:
    A (int&);
    ~A ();

    int bar ();
};

class BufferCheck : public A
{
public:
    BufferCheck (int&, int&);
    ~BufferCheck ();

    int Test (Buffer, Buffer&);
};

#endif
