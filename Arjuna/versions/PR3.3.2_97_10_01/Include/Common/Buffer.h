/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Buffer.h,v 1.14.6.1 1996/10/10 12:25:41 ngdp Exp $
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#ifndef STUB
#  include <Common/Buffer_stub.h>
#else

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

#ifndef POINTERMAP_H_
#  include <Common/PointerMap.h>
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
    
    const char *buffer () const;
    size_t length () const;
    BufferState readState () const;
    void reread ();
    void rewrite ();

    Boolean setBuffer (size_t buffSize, void *buffer, Boolean full = TRUE,
		       Boolean shouldDel = TRUE);

    void setState (BufferState);
    
    /* Pointer Manipulation handling */
    
    Boolean shouldPack (const void *ptr, Boolean = FALSE, Uint32 = 0);
    Boolean saveUnpackedPtr (const void *ptr, Int32& ptrIndex);
    Boolean unpackPtr (const void *& ptr, Int32& ptrIndex);

    /* pack operations for standard types */

#ifdef HAVE_BOOLEAN_TYPE
    Boolean pack (bool);
#endif
    
    Boolean pack (char);
    Boolean pack (unsigned char);
    Boolean pack (short);
    Boolean pack (unsigned short);
    Boolean pack (int);
    Boolean pack (unsigned int);
    Boolean pack (long);
    Boolean pack (unsigned long);
#ifdef HAVE_LONG_LONG
    Boolean pack (long long);
    Boolean pack (unsigned long long);
#endif
    Boolean pack (float);
    Boolean pack (double);
    Boolean packMappedString (const char *);
    Boolean packNonMappedString (const char *);
    Boolean packMappedBlock (const void *, const size_t);
    Boolean packNonMappedBlock (const void *, const size_t);

#ifdef HAVE_BOOLEAN_TYPE
    Boolean unpack (bool&);
#endif
    Boolean unpack (char&);
    Boolean unpack (unsigned char&);
    Boolean unpack (int&);
    Boolean unpack (unsigned int&);
    Boolean unpack (long&);
    Boolean unpack (unsigned long&);
#ifdef HAVE_LONG_LONG
    Boolean unpack (long long&);
    Boolean unpack (unsigned long long&);
#endif 
    Boolean unpack (short&);
    Boolean unpack (unsigned short&);
    Boolean unpack (float&);
    Boolean unpack (double&);
    Boolean unpack (char *&);
    Boolean unpack (void *&, size_t&);
    
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
    Boolean initBuffer ();
    Boolean pack32bits (Uint32);
    Boolean pack64bits (Uint64);
    Boolean packBlock (const void *, size_t, Boolean);
    Boolean unpack32bits (Uint32&);
    Boolean unpack64bits (Uint64&);
    Boolean unpackBlock (void *&, size_t&);
    Boolean realign (char *&);    
    void setLength (size_t);
    Boolean writeHeader ();
    
    /* private state variables */

    char shortSize;			/* bytes in a short */
    char intSize;			/* bytes in an int */
    char longSize;			/* bytes in a long */
    char longlongSize;			/* bytes in a long long */
    
    BufferState buffState;
    char *bufferEnd;
    char *bufferStart;
    int rereadOffset;
    
    size_t chunkSize;
    char *packPoint;
    char *unpackPoint;

    Boolean shouldDelete;
    Boolean readOnly;
    
    PointerMap packPointers;
    PointerMap unpackPointers;
};

#include <Common/Buffer.n>

#  endif
#endif
