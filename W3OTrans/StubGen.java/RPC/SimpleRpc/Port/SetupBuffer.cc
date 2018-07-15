/*
 * Copyright 1995, 1996,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 *
 * $Id: SetupBuffer.cc,v 1.1 1996/02/12 14:42:27 nmcl Exp $
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
