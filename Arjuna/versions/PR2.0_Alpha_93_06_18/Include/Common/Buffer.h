/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Buffer.h,v 1.21 1993/09/26 18:19:22 n048z Exp $
 */

#ifndef BUFFER_H_

#ifndef STUB
#  include <Common/Buffer_stub.h>
#else

#define BUFFER_H_ 

/*
 *
 * Class that implements the marshalling operations for the primitive C types
 * supported by C++ (with certain obvious exceptions).
 * The overloaded operations pack and unpack save/restore the primitive
 * C types to/from memory which is allocated in chunks that default to the
 * size defined below. This optimisation significantly improves the
 * performance of this class.
 * Buffers maintain an internal state variable that determines the
 * state of the buffer. Only when the state is 'good()' will pack/unpack
 * operations be performed.
 *
 */

#ifndef STDDEF_H_
#  include <System/stddef.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

// @NoMarshall
enum BufferState { BUFFER_GOOD, BUFFER_BAD };     

/* Avoid including iostream.h if at all possible */

// @NoRemote, @NoMarshall
class ostream;

// @NoRemote, @NoMarshall
class RpcBuffer;


static const size_t DEFAULT_CHUNK_SIZE = 512;

// @NoRemote, @UserMarshall
class Buffer
{
public:
    /* Constructors & destructor */
    
    Buffer (size_t buffSize = DEFAULT_CHUNK_SIZE);
    Buffer (size_t buffSize, char *buffer, Boolean full = TRUE,
	    Boolean shouldDel= TRUE);
    Buffer (const Buffer& copyFrom);
    virtual ~Buffer ();

    Boolean sizeup (size_t);
    
    /* non-virtual member functions and operators */
    
    char *buffer () const;
    size_t length () const;
    BufferState readState () const;
    void reread ();
    void rewrite ();

    Boolean setBuffer (size_t buffSize, char *buffer, Boolean full = TRUE,
		       Boolean shouldDel = TRUE);
    
    /* pack and unpack operations for standard types */
    
    Boolean pack (const char);
    Boolean pack (const unsigned char);
    Boolean pack (const int);
    Boolean pack (const unsigned int);
    Boolean pack (const long);
    Boolean pack (const unsigned long);
    Boolean pack (const short);
    Boolean pack (const unsigned short);
    Boolean pack (const float);
    Boolean pack (const double);
    Boolean pack (const char *);		   /* null terminated string */
    Boolean pack (const char *, const size_t);  /* contiguous block of memory */
    
    Boolean unpack (char&);
    Boolean unpack (unsigned char&);
    Boolean unpack (int&);
    Boolean unpack (unsigned int&);
    Boolean unpack (long&);
    Boolean unpack (unsigned long&);
    Boolean unpack (short&);
    Boolean unpack (unsigned short&);
    Boolean unpack (float&);
    Boolean unpack (double&);
    Boolean unpack (char *&);
    Boolean unpack (char *&, size_t&);
    
    /* other operators */
    
    Buffer& operator= (const Buffer&);	/* assignment that copies the buffer */
    Buffer& operator+= (const Buffer&);	/* concatentation operator */
    Boolean operator== (const Buffer&) const; /* equality operator */
    
    /* virtual member functions and operators */
 
    /* Allow Buffers to be packed into other Buffers */

    virtual Boolean packInto (Buffer&) const;
    virtual Boolean unpackFrom (Buffer&);
    virtual Boolean appendUnused (const Buffer&);
   
    /* Rpc marshalling */

    virtual void marshall (RpcBuffer&) const;
    virtual void unmarshall (RpcBuffer&);
    
    virtual ostream& print (ostream&) const;

private:
    /* private member functions */
    
    void copybuff (const Buffer&);
    Boolean realign (char *&);    
    void setLength (size_t);
    
    /* private state variables */

    BufferState buffState;
    char *bufferEnd;
    char *bufferStart;
    size_t chunkSize;
    char *packPoint;
    char *unpackPoint;

    Boolean shouldDelete;
};

#include <Common/Buffer.n>

#  endif
#endif
