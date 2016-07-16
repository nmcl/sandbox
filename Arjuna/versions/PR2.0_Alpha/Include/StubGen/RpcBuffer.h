/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RpcBuffer.h,v 1.13 1993/03/22 09:38:33 ngdp Exp $
 */

#ifndef RPCBUFFER_H_
#define RPCBUFFER_H_

/*
 *
 * Class to handle RPC buffering. Based upon the standard Arjuna Buffer
 * class
 *     
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

static const int MAP_TBL_SIZE = 255;	/* power of 2 minus 1 */

class RpcBuffer : public Buffer
{
public:
    /* Constructors and destructor */

    RpcBuffer ();
    RpcBuffer (long initVal);
    RpcBuffer (const RpcBuffer& copyFrom);
    RpcBuffer (const RpcBuffer& copyFrom, long initVal);
    virtual ~RpcBuffer ();

    Boolean addPtr (const void *ptr, long& ptrIndex);
    void clearMap (Boolean = FALSE);
    Boolean unpackPtr (const void *&ptr, long& ptrIndex);
    Boolean packPtr (const void *ptr);
    const void *pointerAt (long ptrIndex) const;

    /* Standard marshalling operations */

    RpcBuffer& operator<< (const char);
    RpcBuffer& operator<< (const unsigned char);
    RpcBuffer& operator<< (const short);
    RpcBuffer& operator<< (const unsigned short);
    RpcBuffer& operator<< (const int);
    RpcBuffer& operator<< (const unsigned int);
    RpcBuffer& operator<< (const long);
    RpcBuffer& operator<< (const unsigned long);
    RpcBuffer& operator<< (const float);
    RpcBuffer& operator<< (const double);

    RpcBuffer& operator<< (const char *);
    RpcBuffer& operator<< (const unsigned char*);
    RpcBuffer& operator<< (const short*);
    RpcBuffer& operator<< (const unsigned short*);
    RpcBuffer& operator<< (const int*);
    RpcBuffer& operator<< (const unsigned int*);
    RpcBuffer& operator<< (const long*);
    RpcBuffer& operator<< (const unsigned long*);
    RpcBuffer& operator<< (const float*);
    RpcBuffer& operator<< (const double*);

    /* Standard unmarshalling operations */

    RpcBuffer& operator>> (char&);
    RpcBuffer& operator>> (unsigned char&);
    RpcBuffer& operator>> (short&);
    RpcBuffer& operator>> (unsigned short&);
    RpcBuffer& operator>> (int&);
    RpcBuffer& operator>> (unsigned int&);
    RpcBuffer& operator>> (long&);
    RpcBuffer& operator>> (unsigned long&);
    RpcBuffer& operator>> (float&);
    RpcBuffer& operator>> (double&);

    RpcBuffer& operator>> (char *&);
    RpcBuffer& operator>> (short*&);
    RpcBuffer& operator>> (unsigned short*&);
    RpcBuffer& operator>> (int*&);
    RpcBuffer& operator>> (unsigned int*&);
    RpcBuffer& operator>> (long*&);
    RpcBuffer& operator>> (unsigned long*&);
    RpcBuffer& operator>> (float*&);
    RpcBuffer& operator>> (double*&);

private:
    long mapPtr (const void *ptr);

    int mapEntries;
    const void *ptrMapping[MAP_TBL_SIZE];

};

/*
 * (Un)Marshalling operations. Those that can be easily inlined are
 * declared in RpcBuffer.n.
 * All others can be found in RpcBuffer.cc
 */

class ObjectState;
class Uid;

extern RpcBuffer& operator<< (RpcBuffer&, const Buffer&);
extern RpcBuffer& operator<< (RpcBuffer&, const Buffer *);
extern RpcBuffer& operator<< (RpcBuffer&, const Uid&);
extern RpcBuffer& operator<< (RpcBuffer&, const Uid *);

extern RpcBuffer& operator>> (RpcBuffer&, Buffer&);
extern RpcBuffer& operator>> (RpcBuffer&, Buffer*&);
extern RpcBuffer& operator>> (RpcBuffer&, ObjectState&);
extern RpcBuffer& operator>> (RpcBuffer&, ObjectState*&);
extern RpcBuffer& operator>> (RpcBuffer&, Uid&);
extern RpcBuffer& operator>> (RpcBuffer&, Uid*&);

#include <StubGen/RpcBuffer.n>

#endif
