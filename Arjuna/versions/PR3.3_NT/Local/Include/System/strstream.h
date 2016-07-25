/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: strstream.h,v 1.3 1996/04/15 14:18:01 nmcl Exp $
 */

#ifndef STRSTREAM_H_
#define STRSTREAM_H_ 

#ifdef STUB
#  pragma @NoRename
#endif

// @NoRemote @NoMarshall
class strstreambuf;

// @NoRemote @NoMarshall
class strstreambase;

// @NoRemote @NoMarshall
class istrstream;

// @NoRemote @NoMarshall
class ostrstream;

// @NoRemote @NoMarshall
class strsteam;

#ifndef WIN32
#  include <strstream.h>
#else
#  include <strstrea.h>
#endif

#endif
