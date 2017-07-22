/*
 * Copyright 1990, 1991, 1992
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 *
 * Permission to use, copy, modify and distribute this program for
 * evaluation, teaching and/or research purposes only and without fee is
 * hereby granted, providing that this copyright and permission notice
 * appear on all copies and supporting documentation, and that similar
 * conditions are imposed on any individual or organization to whom the
 * program is distributed. The University of Newcastle upon Tyne makes no
 * representation about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * $Id: Buffer.h,v 1.7 1992/02/19 16:37:08 ngdp Exp $
 */

#ifndef BUFFER_H_
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

#include <stddef.h>

#include <Common/Boolean.h>

enum Buffer_State { BUFFER_GOOD, BUFFER_BAD };     

/* Avoid including iostream.h if at all possible */

class ostream;

static const int DEFAULT_CHUNK_SIZE = 512;
static const int ALIGNMENT = 4;

class Buffer
{
public:
    /* Constructors & destructor */
    
    Buffer (size_t = DEFAULT_CHUNK_SIZE);
    Buffer (size_t, char *, Boolean = TRUE, Boolean = TRUE);
    Buffer (const Buffer&);
    ~Buffer ();
    
    /* non-virtual member functions and operators */
    
    char *buffer () const;
    size_t length () const;
    Buffer_State readstate () const;
    void rewind ();
    Boolean set_buffer (size_t, char *, Boolean = TRUE, Boolean = TRUE);
    
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
    Boolean unpack (char* &, Boolean = FALSE);
    Boolean unpack (char *&, size_t&);
    
    /* other operators */
    
    Buffer& operator= (const Buffer&);	/* assignment that copies the buffer */
    Buffer& operator+= (const Buffer&);	/* concatentation operator */
    
    /* virtual member functions and operators */
 
    /* Allow Buffers to be packed into other Buffers */

    virtual Boolean pack_into (Buffer&) const;
    virtual Boolean unpack_from (Buffer&);
   
    virtual ostream& print (ostream&) const;

private:
    /* private member functions */
    
    void copybuff (const Buffer&);
    Boolean realign (char *&);    
    Boolean sizeup (size_t);
    void set_length (size_t);
    
    /* private state variables */

    Buffer_State buff_state;
    char *buffer_end;
    char *buffer_start;
    size_t chunk_size;
    char *pack_point;
    char *unpack_point;

    Boolean should_delete;
};

#endif
