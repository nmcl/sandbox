/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MappedSeg.cc,v 1.2 1995/10/05 08:01:07 ngdp Exp $
 */

/*
 * Simple shared memory segment manager using mmapped files.
 * This will be replaced once Arjuna is moved to a multi-threaded
 * server environment. In order to avoid attaching the SAME region
 * more than once to a process via mmap (and thus get ANOTHER virtual
 * address for the same segment) we chain attached segments onto a
 * per process list which gets searched and the virtual address for the
 * already attached segment is returned if the segment id found.
 */

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef SYS_MMAN_H_
#  include <System/sys/mman.h>
#endif

#ifndef SYS_STAT_H_
#  include <System/sys/stat.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SYS_IPC_H_
#  include <System/sys/ipc.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef ERRORLIST_H_
#  include <Common/ErrorList.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef MAPPEDSEG_H_
#  include "MappedSeg.h"
#endif

struct RegionHead			/* sits at location 0 of region */
{
    int magic;
    int usageCount;
    size_t regionSize;
};

/*
 * per process segment map - describes attributes of an attached segment
 */

struct SegMap
{
    size_t fullSize;			/* size of mapped file INC overhead */
    int id;				/* id of segment */
    int fd;				/* fd of underlying file */
    int usage;				/* number of attaches by this process */
    char *fName;			/* name of supporting file */
    char *addr;				/* address segment attached to */
    SegMap *link;
};

SegMap *MappedSegment::segMapPtr = 0;		/* Head of mapping list */

static const int OVERHEAD = sizeof(struct RegionHead);
static const int MAGIC = 0xeca97531;

static const char RCSid[] = "$Id: MappedSeg.cc,v 1.2 1995/10/05 08:01:07 ngdp Exp $";

static int privateCount = 0;

/*
 * Public constructors and destructor
 */

/*
 * Constructor: Create a mapped segment manager. smKey determines the name
 * of the area (System V style). smSize is the total usable size.
 * More is actually allocated due to the overhead of the segment header.
 * Segments sizes are rounded to page size
 * Segment names are funny as this is intended as a replacement
 * for system V shared memory and so uses the same naming scheme.
 */

MappedSegment::MappedSegment ( key_t smKey, size_t smSize )
                             : segId(-1),
			       segVaddr(0),
			       lastError()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "MappedSegment::MappedSegment(" << smKey << ", " 
	         << smSize << ")" << endl;
    debug_stream.unlock();
    
#endif

    segVaddr = smInit(smKey, smSize);
    
}

/*
 * Create segment manager that attaches to an existing shared segment 
 * rather than creating new one
 */

MappedSegment::MappedSegment ( int smId )
                             : segId(smId),
			       segVaddr(0),
			       lastError()
{    
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "MappedSegment::MappedSegment(" << smId << ")" << endl;
    debug_stream.unlock();
    
#endif

#ifdef HAVE_MMAP
    long size = 0;
    const SegMap *sp = 0;

    if ((sp = locate(smId)) != 0)
    {
	segVaddr = attach(smId, sp->fd, sp->fullSize, 0);
    }
    else
    {
	char fname[256];
	ostrstream name(fname, 256);
	int fd = -1;
	
	name << "/tmp/IPC_" << smId << ends;
	
	if ((fd = open(fname, O_RDWR, 0666)) != -1)
	{
	    struct stat statBuff;
	    
	    if (fstat(fd,&statBuff) == 0)
		segVaddr = attach(smId, fd, statBuff.st_size, fname);
	}
    }
#endif
}

/*
 * Destructor: Clean up on deletion. Detach segment and free if use
 * count has dropped to 0.
 */

MappedSegment::~MappedSegment ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "MappedSegment::~MappedSegment() for segment id "
	         << segId << "" << endl;
    debug_stream.unlock();
    
#endif

#ifdef HAVE_MMAP
    detach(segId);
#endif
}

/*
 * Public non-virtual functions
 */

Boolean MappedSegment::addError ( Error *newError )
{
    return lastError.insert(newError);
}

void MappedSegment::clearErrorList ()
{
    lastError.clearList();
}


/*
 * dumpMem. Write the entire contents of the segment out to a file.
 */

void MappedSegment::dumpMem ( const char *fileName ) const
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "MappedSegment::dumpMem(" << fileName << ")" << endl;
    debug_stream.unlock();
    
#endif

    int fd;
    struct RegionHead *q = (struct RegionHead *) (segVaddr);

    /* Check there is a segment attached. */

    if (segVaddr == 0)
	return;

    fd = ::open((char *)fileName, O_WRONLY | O_CREAT, 0777);

    ::write(fd, segVaddr, (int) q->regionSize + OVERHEAD);
    
    ::close(fd);
}

/*
 * get. Retrieve information from segment. Recovers size bytes into
 * user supplied buffer from segment. Note that 'from' is not a
 * genuine virtual address but an offset from the start of the shared
 * segment. 'to', however is a real process address.
 * Checks to make sure region is big enough to supply the number of
 * bytes requested.
 */

long MappedSegment::get ( SmAddr from, long size, char *to ) const
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "MappedSegment::get(" << from << ", " << size
	         << ", " << (void *)to << ")" << endl;
    debug_stream.unlock();
    
#endif

    long rSize = -1;
    struct RegionHead *q = (struct RegionHead *) (segVaddr);

    if ((segVaddr != 0) && (from >= 0) && (size >= 0))
    {
	if (size > 0)
	{
	    if (from + size <= q->regionSize)
	    {
		::memcpy(to, from + segVaddr + OVERHEAD, (int) size);
		rSize = size;
	    }
	}
	else
	    rSize = 0;
    }
    
    return rSize;
}


/*
 * put. Inverse of get with same provisos.
 */

long MappedSegment::put ( SmAddr to, long size, const char *from )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "MappedSegment::put(" << to << ", " << size
	         << ", " << (void *)from << ")" << endl;
    debug_stream.unlock();
    
#endif

    long rSize = -1;
    
#ifdef HAVE_MMAP
    struct RegionHead *q = (struct RegionHead *) (segVaddr);

    if ((segVaddr != 0) && (to >= 0) && (size >= 0))
    {
	if (size > 0)
	{
	    if ((mprotect(segVaddr, q->regionSize + OVERHEAD, PROT_READ|PROT_WRITE) == 0))
	    {
		if (to + size <= q->regionSize)
		{
		    ::memcpy(to + segVaddr + OVERHEAD, from, (int) size);
		    rSize = size;
		}
		mprotect(segVaddr, q->regionSize + OVERHEAD, PROT_READ);
	    }
	}
	else
	    rSize = 0;
    }
#endif
    return rSize;
}

int MappedSegment::shmid () const
{
    return segId;
}

/*
 * Protected operations
 */

/*
 * Constructor - This one does no explicit initialisation of the area -
 * that job is the perogative of the derived class (by calling smInit).
 */

MappedSegment::MappedSegment ()
                             : segId(0),
			       segVaddr(0),
			       lastError()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_SHMEM << VIS_PROTECTED;
    debug_stream << "MappedSegment::MappedSegment()" << endl;
    debug_stream.unlock();
    
#endif
}

/*
 * Return segment virtual address (adjusting for segment header).
 */

char *MappedSegment::getSegVaddr () const
{
    if (segVaddr != 0)
	return segVaddr + OVERHEAD;
    else
	return 0;
}

/*
 * Private non-virtual functions
 */

/*
 * smInit. Initialise the shared memory segment manager (and the
 * actual segment if this is the call that actually creates it in the
 * system). The segment is laid out as follows:
 *
 * +-------------+--------------------------------------------------+
 * |             |                                                  |
 * | control     | unrestricted                                     |
 * |  info       |  usage                                           |
 * |             |                                                  |
 * +-------------+--------------------------------------------------+
 *               |                                                  |
 *               +-------------- segSize bytes ---------------------+
 *
 * All addresses returned and used are offsets from the start of the
 * memory area (the virtual address of which is kept private). Thus
 * only the operations get and put can access the region.
 * The unrestricted area has no control over it and can be used for
 * anything.
 */

char *MappedSegment::smInit ( key_t smKey, size_t smSize )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_SHMEM << VIS_PRIVATE;
    debug_stream << "MappedSegment::smInit(" << smKey << ", " 
		 << smSize << ")" << endl;
    debug_stream.unlock();
    
#endif
    
    char *start = 0;

#ifdef HAVE_MMAP
    struct RegionHead *q = 0;
    Boolean initialised = TRUE;
    long size = smSize + OVERHEAD;
    long pageSize = 0;
    char fname[256];
    ostrstream name(fname, 256);
    int fd = -1;
    
#ifdef SYSV
    pageSize = sysconf(_SC_PAGESIZE);
#else
    pageSize = getpagesize();
#endif
    
    /* round up to nearest multiple of pageSize */

    if (size % pageSize)
        size += pageSize - (size % pageSize);

    if (smKey == IPC_PRIVATE)
    {
	/* generate a new private key */

	smKey = (getpid() << 16) | privateCount++;
    }
    
    /*
     * Try for exclusive creation of segment. If this fails see if this
     * was because the segment already existed. If so attach to it.
     * Otherwise there's an error.
     */

    name << "/tmp/IPC_" << smKey << ends;
    
    if ((fd = open(fname, O_RDWR | O_CREAT | O_EXCL, 0666)) != -1)
    {
	Boolean createOk = TRUE;
	char *zeroBuffer = new char[pageSize];

	if (zeroBuffer)
	{
	    ::memset(zeroBuffer, 0, pageSize);

	    /* force zero fill of file to correct size */
	    
	    for (int i = 0; i < (size / pageSize); i++)
		if ((write(fd, zeroBuffer, pageSize) != pageSize))
		{
		    createOk = FALSE;
		    break;
		}

	    delete [] zeroBuffer;
	}

	if (createOk)
	{
	    RegionHead t;

	    t.usageCount = 0;
	    t.regionSize = size - OVERHEAD;
	    t.magic = MAGIC;
	    if ((lseek(fd,0,SEEK_SET) == 0) &&
		(write(fd, (char *)&t, sizeof(t)) == sizeof(t)))
		createOk = TRUE;
	}

	if (!createOk)
	{
	    close(fd);
	    fd = -1;

	    unlink(fname);
	}
    }
    else
    {
        int savedErrno = errno;
	
	if ((errno != EEXIST) ||
	    (fd = open(fname, O_RDWR, 0666)) == -1)
	{
	    char buff[256];
	    ostrstream temp(buff, 256);
	    Error *error;
	    
	    clearErrorList();
	    
	    temp << "MappedSegment::MappedSegment() failed: ";
            if (savedErrno != EEXIST)
		temp << " Could not create ";
	    else
	        temp << " Could not attach ";

	    temp << "mapping with key: " << smKey << " errno: " << errno << ends;

	    error = new Error(FAC_SHMEM, SHM_CREATE_FAILED, buff);

	    addError(error);
            return 0;
        }
    }

    /* File open - attempt to map it */
    
    if (fd != -1)
    {
	if ((start = attach(smKey, fd, size, fname)) == 0)	/* map segment */
	{
	    Error *error;

	    clearErrorList();
	    error = new Error(FAC_SHMEM, SHM_ATTACH_FAILED,
			      "MappedSegment::attach() failed");

	    addError(error);
	    close(fd);
	    fd = -1;
	}
	else
	{
	    segId = smKey;
	    segVaddr = start;
	}
    }
#endif    
    return start;
}

/*
 * Attaching to an existing segment. If we have attached the segment
 * already in this process it is in the segment mapping list. If so
 * we update the usage count and return the virtual address found there.
 * If not found we do a mmap and build a new entry.
 */

char *MappedSegment::attach ( int shmId, int shmFd, size_t shmSize,
			      const char *name )
{
#ifdef HAVE_MMAP
    int savedErrno = 0;
    SegMap *p = MappedSegment::segMapPtr;

    /* Scan existing list for id */

    while ( p != 0 )
    {
	if (p->id == shmId)
	    break;

	p = p->link;
    }

    if (p == 0)
    {
	/* reached end of list => first attach by this process */

	Boolean magicOk = FALSE;
	Boolean sizeOk = FALSE;
	struct stat statBuff;
	char *start = 0;
	int retry = 10;

	/*
	 * ensure underlying file is correct size - this
	 * is a wait since another process could be doing the
	 * init
	 */
	
	statBuff.st_size = 0;

	while ((!sizeOk) && (retry > 0))
	{
	    if ((fstat(shmFd, &statBuff) == 0) && (statBuff.st_size == shmSize))
		    sizeOk = TRUE;
	    else
	    {
		sleep(1);
		retry--;
	    }
	} 

	/*
	 * underlying file should now be correct size
	 * if it is wait for magic flag to be written - this
	 * is a another wait since another process could be doing the
	 * init
	 */
	
	if (sizeOk)
	{
	    if ((start = (char*)::mmap(0, shmSize, PROT_READ|PROT_WRITE, MAP_SHARED, shmFd, 0)) != (char *) -1)
	    {
		RegionHead *q = (RegionHead *)start;
		
		/* wait for magic flag to be set */

		retry = 10;

		while ((!magicOk) && (retry > 0))
		{
		    if (q->magic == MAGIC)
			magicOk = TRUE;
		    else
		    {
			sleep(1);
			retry--;
		    }
		}
		
		if ((magicOk) && ((p = new SegMap) != 0))
		{
		    /* successful mmap - add to mapping table */

		    p->usage = 0;
		    p->fullSize = shmSize;
		    p->addr = start;
		    p->id = shmId;
		    p->fd = shmFd;
		    p->fName = clone(name);
		    p->link = MappedSegment::segMapPtr;
		    MappedSegment::segMapPtr = p;
		    
		    /* update usage count in mapped file */
		
		    q->usageCount++;

		    /* Turn off memory accessability */
		    
		    mprotect(start, shmSize, PROT_READ);
		}
		else
		{
		    munmap(start, shmSize);
		}
	    }
	    else
		savedErrno = errno;
	}

	if ((!sizeOk) || (!magicOk))
	{
	    char buff[256];
	    ostrstream temp(buff, 256);
	    Error *error = 0;

	    clearErrorList();

	    temp << "MappedSegment::attach() failed for segment with id: ";
	    temp << shmId << " errno: " << savedErrno << ends;

	    error = new Error(FAC_SHMEM, SHM_ATTACH_FAILED, buff);
	    addError(error);
	}	
    }

    if (p != 0)
    {
	p->usage++;
	return p->addr;
    }
    else
#endif
	return 0;

}

/*
 * Detach a segment. Update the usage count in the mapping table
 * and if it drops to zero check the usage count on the segment
 * itself. If that is zero actually remove the segment from the system.
 */

void MappedSegment::detach ( int shmId )
{
#ifdef HAVE_MMAP
    SegMap *p = MappedSegment::segMapPtr;
    SegMap *q = p;

    /* Scan existing list for id */

    while ( p != 0 )
    {
	if (p->id == shmId)
	    break;

	q = p;
	p = p->link;
    }

    if (p != 0)
    {
	/* found map entry - update it */

	p->usage--;
	if (p->usage == 0)
	{
	    /* Usage by this process is now zero - decrement count in file */

	    if (p->addr != 0)
	    {
		RegionHead *r = (RegionHead*)p->addr;

		if (mprotect(p->addr, p->fullSize, PROT_READ|PROT_WRITE))
		{
		    if ((--r->usageCount == 0) && p->fName)
			unlink(p->fName);
		    
		    mprotect(p->addr, p->fullSize, PROT_READ);
		}

		munmap(p->addr, p->fullSize);

		if (p->fd != -1)
		    close(p->fd);
	    }

	    /* Thow away map entry */

	    if (p == MappedSegment::segMapPtr)
		MappedSegment::segMapPtr = p->link;
	    else
	    {
		q->link = p->link;
	    }

	    if (p->fName)
		delete [] p->fName;

	    delete p;
	}
    }
#endif
}

const SegMap *MappedSegment::locate ( int shmId ) const
{
    SegMap *p = MappedSegment::segMapPtr;

    /* Scan existing list for id */

    while ( p != 0 )
    {
	if (p->id == shmId)
	    break;

	p = p->link;
    }

    return p;
}

int MappedSegment::usage () const
{
    int myUsage = 0;
    int procUsage = 0;

#ifdef HAVE_MMAP
    SegMap *p = MappedSegment::segMapPtr;
    RegionHead *q = 0;

    /* Scan existing list for id */

    while ( p != 0 )
    {
	if (p->id == segId)
	{
	    myUsage = p->usage - 1;

	    if (p->addr)
	    {
		q = (RegionHead *)p->addr;
		procUsage = q->usageCount;
	    }
	    
	    break;
	}

	p = p->link;
    }
#endif
    return procUsage + myUsage;
}

#ifdef NO_INLINES
#  define MAPPEDSEG_CC_
#  include "MappedSeg.n"
#  undef MAPPEDSEG_CC_
#endif
