/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SharedSeg.h,v 1.3 1995/06/26 13:09:04 ngdp Exp $
 */

#ifndef SHAREDSEG_H_
#define SHAREDSEG_H_

/*
 *
 * Shared Segment class. Simple System V shared memory handler.
 * Controls access to a chunk of shared memory, which can only be
 * manipulated through the get and put operations since the virtual
 * address at which the memory is attached is not revealed to the
 * caller.
 *
 */

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_IPC_H_
#  include <System/sys/ipc.h>
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

class SharedSegment
{
public:
    /* Constructors and destructor */
    
    SharedSegment (const key_t smKey, size_t smSize);
    SharedSegment (int smId);
    virtual ~SharedSegment ();

    /* non-virtual member functions */

    Boolean addError (Error *);
    void clearErrorList ();
    ErrorList& getErrorList ();

    void dumpMem (const char *fileName) const;
    long get (SmAddr from, long size, char *to) const;
    long put (SmAddr to, long size, const char *from);

    Boolean isValid () const;
    Boolean IsNew () const;
    int shmid () const;
    int usage () const;

protected:
    SharedSegment ();

    char *getSegVaddr () const;
    char *smInit (const key_t smKey, size_t smSize); /* basic init */

private:
    char *attach (int shmId);
    void detach (int shmId);

    /* private state variables */

    static SegMap *segMapPtr;	        /* head of list of attached memory */

    int segId;				/* shared segment id */    
    char *segVaddr;			/* virtual address of segment */    
    Boolean isNew;                      /* is this the creation of the segment? */

    ErrorList lastError;
};

#include "SharedSeg.n"

#endif
