/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MappedFile.h,v 1.2 1995/08/31 09:24:49 ngdp Exp $
 */

#ifndef MAPPEDFILE_H_
#define MAPPEDFILE_H_

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

const off_t INITIAL_SIZE = 8192;

// @NoRemote, @NoMarshall
class MappedFile
{
    friend SIG_FUNC_TYPE segvHandler;
    friend SIG_FUNC_TYPE sigbusHandler;
    
public:
    MappedFile (const char *, off_t = INITIAL_SIZE);
    virtual ~MappedFile ();

    Boolean isValid () const;
    Boolean get (char *, off_t, size_t);
    Boolean put (const char *, off_t, size_t);
    Boolean sizeUp (off_t);
    Boolean sync ();
    Boolean truncate (off_t);
    
private: 

    off_t currentSize;			/* size of underlying file */
    int fd;
    char *mappedMemory;			/* mapped address */
    
    Uint32 pageSize;			/* system page size */

    /* signal handling */
    
    SIG_ARGTYPE oldHandler;
    static Boolean signalSeen;
};
#endif
