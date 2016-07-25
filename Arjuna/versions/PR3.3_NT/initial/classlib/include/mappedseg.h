/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MappedSeg.h,v 1.2 1995/10/06 08:29:52 ngdp Exp $
 */

#ifndef MAPPEDSEG_H_
#define MAPPEDSEG_H_

/*
 *
 * Shared Segment class. Simple shared memory handler based on mmap.
 * Controls access to a chunk of shared memory, which can only be
 * manipulated through the get and put operations since the virtual
 * address at which the memory is attached is not revealed to the
 * caller.
 *
 */

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef WIN32

#ifndef SYS_IPC_H_
#  include <System/sys/ipc.h>
#endif

#endif

#ifndef STDDEF_H_
#  include <System/stddef.h>
#endif

#ifndef ERRORLIST_H_
#  include <Common/ErrorList.h>
#endif

struct SegMap;     // xlC bug or ANSI bug??

typedef long SmAddr;

class Error;

class MappedSegment
{
public:
    /* Constructors and destructor */
    
    MappedSegment (key_t smKey, size_t smSize);
    MappedSegment (int smId);
    virtual ~MappedSegment ();

    /* non-virtual member functions */

    Boolean addError (Error *);
    void clearErrorList ();
    ErrorList& getErrorList ();

    void dumpMem (const char *fileName) const;
    long get (SmAddr from, long size, char *to) const;
    long put (SmAddr to, long size, const char *from);

    Boolean isValid () const;
    int shmid () const;
    int usage () const;

protected:
    MappedSegment ();

    char *getSegVaddr () const;
    char *smInit (key_t smKey, size_t smSize); /* basic init */

private:
    char *attach (int shmId, int fd, size_t smSize, const char *);
    void detach (int shmId);
    const SegMap *locate (int shmId) const;

    /* private state variables */

    static SegMap *segMapPtr;	        /* head of list of attached memory */

    int segId;				/* shared segment id */    
    char *segVaddr;			/* virtual address of segment */    

    ErrorList lastError;
};

#include "MappedSeg.n"

#endif
