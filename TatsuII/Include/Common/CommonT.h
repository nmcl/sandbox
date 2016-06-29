/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CommonT.h,v 1.1 1997/06/09 19:51:48 nmcl Exp $
 */

#ifndef COMMONT_H_
#define COMMONT_H_

#ifdef STUB
#  pragma @NoRename
#endif

/*
 * Common and useful definitions
 * 
 */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

/* simple type definitions */

typedef char *TypeName;
typedef void *AnyType;

/* These are true for most 32 bit machines/compilers */

typedef unsigned char Uint8;
typedef signed char Int8;		/* in case plain 'char' is unsigned */
typedef unsigned short Uint16;
typedef short Int16;
typedef unsigned int Uint32;
typedef int Int32;

/* Here it gets tricky! These are typedefs for 64 bit ints */

#ifdef LONG_IS_64BITS
typedef unsigned long Uint64;
typedef long Int64;
#else
#  ifdef HAVE_LONG_LONG
typedef unsigned long long Uint64;
typedef long long Int64;
#  else
#    ifndef LITTLE_ENDIAN_MC
// @UserMarshall, @NoRemote
struct Int64
{
    Int32 hi;
    Int32 lo;
};

// @UserMarshall, @NoRemote
struct Uint64
{
    Uint32 hi;
    Uint32 lo;
    
};
#    else
// @UserMarshall, @NoRemote
struct Int64
{
    Int32 lo;
    Int32 hi;
};

// @UserMarshall, @NoRemote
struct Uint64
{
    Uint32 lo;
    Uint32 hi;
};
#    endif
#  endif
#endif

#endif
