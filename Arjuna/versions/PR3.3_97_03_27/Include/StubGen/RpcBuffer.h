/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RpcBuffer.h,v 1.6 1994/12/09 16:15:25 ngdp Exp $
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

class RpcBuffer : public Buffer
{
public:
    /* Constructors and destructor */

    RpcBuffer ();
    RpcBuffer (Int32 initVal);
    RpcBuffer (const RpcBuffer& copyFrom);
    RpcBuffer (const RpcBuffer& copyFrom, Int32 initVal);
    virtual ~RpcBuffer ();

    /* Standard marshalling operations */

#ifdef HAVE_BOOLEAN_TYPE
    RpcBuffer& operator<< (bool);
#endif
    RpcBuffer& operator<< (char);
    RpcBuffer& operator<< (unsigned char);
    RpcBuffer& operator<< (short);
    RpcBuffer& operator<< (unsigned short);
    RpcBuffer& operator<< (int);
    RpcBuffer& operator<< (unsigned int);
    RpcBuffer& operator<< (long);
    RpcBuffer& operator<< (unsigned long);
    RpcBuffer& operator<< (float);
    RpcBuffer& operator<< (double);


#ifdef HAVE_BOOLEAN_TYPE
    RpcBuffer& operator<< (const bool *);
#endif
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


#ifdef HAVE_BOOLEAN_TYPE
    RpcBuffer& operator>> (bool&);
#endif
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
    
#ifdef HAVE_BOOLEAN_TYPE
    RpcBuffer& operator>> (bool *&);
#endif
    RpcBuffer& operator>> (char *&);
    RpcBuffer& operator>> (short*&);
    RpcBuffer& operator>> (unsigned short*&);
    RpcBuffer& operator>> (int*&);
    RpcBuffer& operator>> (unsigned int*&);
    RpcBuffer& operator>> (long*&);
    RpcBuffer& operator>> (unsigned long*&);
    RpcBuffer& operator>> (float*&);
    RpcBuffer& operator>> (double*&);
    
    /* Comparison operations */

    Boolean operator!= (const RpcBuffer&) const;
};

#include <StubGen/RpcBuffer.n>

#endif
