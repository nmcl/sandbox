/*
 * Copyright 1995, 1996-1998,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 *
 * $Id: SetupBuffer.cc,v 1.2 1998/11/20 08:28:43 nmcl Exp $
 */

#ifndef SETUPBUFFER_H_
#  include "SetupBuffer.h"
#endif

SetupBuffer::SetupBuffer ()
{
    for (int i = 0; i < MSG_MAXIOVLEN; i++)
	bufferSize[i] = htonl(0);

    packetNumber = htonl(0);
    endOfMessage = htonl(0);
}

SetupBuffer::~SetupBuffer ()
{
}
