/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $id:$
 */

#ifndef TRANSPORTER_H_
#define TRANSPORTER_H_

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

class Transporter
{
public:
    Transporter (int&);
    ~Transporter ();

    Buffer Get (Buffer, int&);
};

#endif
