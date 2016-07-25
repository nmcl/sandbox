/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SharedSeg.cc,v 1.9 1995/10/02 15:13:43 ngdp Exp $
 */

/*
 * Simple shared memory segment manager. This will be replaced once
 * Arjuna is moved to a multi-threaded server environment. In order
 * to avoid attaching the SAME region more than once to a process
 * via shmat (and thus get ANOTHER virtual address for the same segment)
 * we chain attached segments onto a per process list which gets searched
 * and the virtual address for the already attached segment is returned
 * if the segment id found.
 * This mess is because HP-UX only allows (by default) 10 calls to shmat
 * in any process - even if attaching the same segment again.
 */

/*
 * Do not move this include!
 */

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef WIN32

#ifndef SYS_IPC_H_
#  include <System/sys/ipc.h>
#endif

#ifndef SYS_SHM_H_
#  include <System/sys/shm.h>
#endif

#ifndef SYS_FILE_H_
#  include <System/sys/file.h>
#endif

#else

#ifndef WINDOWS_H_
#  include <System/windows.h>
#endif

#include <System/io.h>
#include <System/stdio.h>

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

#if defined(WIN32) && !defined(UID_H_)
#  include <Common/Uid.h>
#endif

#ifndef SHAREDSEG_H_
#  include "SharedSeg.h"
#endif

struct RegionHead			/* sits at location 0 of region */
{
    key_t regionKey;
    size_t regionSize;			/* total usable size of shared */
								/* memory region */ 
};

/*
 * per process segment map - describes attributes of an attached segment
 */

struct SegMap
{
#ifndef WIN32
    int id;				/* shared memory id of segment */
#else
	HANDLE id;
#endif
    int usage;				/* number of attaches by this process */
    char *addr;				/* address segment attached to */
    SegMap *link;
};

SegMap *SharedSegment::segMapPtr = 0;		/* Head of mapping list */

static const int OVERHEAD = sizeof(struct RegionHead);

static const char RCSid[] = "$Id: SharedSeg.cc,v 1.9 1995/10/02 15:13:43 ngdp Exp $";

/*
 * Public constructors and destructor
 */

/*
 * Constructor: Create a segment manager. smKey determines the name
 * of the area (System V style). smSize is the total usable size.
 * More is actually allocated due to the overhead of the segment header.
 */

SharedSegment::SharedSegment ( const key_t smKey, size_t smSize )
#ifndef WIN32
                             : segId(-1),
#else
							 : segId(0),
							 fileName(0),
#endif
			       segVaddr(0),
			       isNew(FALSE),
			       lastError()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "SharedSegment::SharedSegment(" << smKey << ", " 
	         << smSize << ")" << endl;
    debug_stream.unlock();
    
#endif

    segVaddr = smInit(smKey, smSize);
    
}

/*
 * Create segment manager that attaches to an existing shared segment 
 * rather than creating new one
 */

#ifndef WIN32
SharedSegment::SharedSegment ( int smId )
                             : segId(smId),
#else
SharedSegment::SharedSegment ( const char* smId)
							 : segId(0),
							   fileName(0),
#endif
			       segVaddr(0),
			       isNew(FALSE),
			       lastError()
{    
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "SharedSegment::SharedSegment(" << smId << ")" << endl;
    debug_stream.unlock();
    
#endif

	segId = ::strdup(smId);

	segHandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, segId);
    segVaddr = attach(segHandle);
}

/*
 * Destructor: Clean up on deletion. Detach segment and free if use
 * count has dropped to 0.
 */

SharedSegment::~SharedSegment ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "SharedSegment::~SharedSegment() for segment id "
	         << segId << "" << endl;
    debug_stream.unlock();
    
#endif

	cout << "in ~SharedSegment" << endl;

    detach(segHandle);

#ifdef WIN32
	if (fileName)
		::delete [] fileName;
	if (segId)
		::delete [] segId;
#endif
}

/*
 * Public non-virtual functions
 */

Boolean SharedSegment::addError ( Error *newError )
{
    return lastError.insert(newError);
}

void SharedSegment::clearErrorList ()
{
    lastError.clearList();
}


/*
 * dumpMem. Write the entire contents of the segment out to a file.
 */

void SharedSegment::dumpMem ( const char *fileName ) const
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "SharedSegment::dumpMem(" << fileName << ")" << endl;
    debug_stream.unlock();
    
#endif

    int fd;
    struct RegionHead *q = (struct RegionHead *) (segVaddr);

    /* Check there is a segment attached. */

    if (segVaddr == 0)
		return;

    fd = ::open((char *)fileName, O_WRONLY | O_CREAT, 0777);
    ::write(fd, segVaddr, (int) q->regionSize + OVERHEAD);
    close(fd);
}

/*
 * get. Retrieve information from segment. Recovers size bytes into
 * user supplied buffer from segment. Note that 'from' is not a
 * genuine virtual address but an offset from the start of the shared
 * segment. 'to', however is a real process address.
 * Checks to make sure region is big enough to supply the number of
 * bytes requested.
 */

long SharedSegment::get ( SmAddr from, size_t size, char *to ) const
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "SharedSegment::get(" << from << ", " << size
	         << ", " << (void *)to << ")" << endl;
    debug_stream.unlock();
    
#endif

    struct RegionHead *q = (struct RegionHead *) (segVaddr);

    if (segVaddr == 0)
		return -1;
    
    if (from + size > q->regionSize)
		return -1;

    if ((from < 0) || (size < 0))
		return -1;
    
    ::memcpy(to, from + segVaddr + OVERHEAD, (int) size);
 
    return size;
}


/*
 * put. Inverse of get with same provisos.
 */

long SharedSegment::put ( SmAddr to, size_t size, const char *from )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "SharedSegment::put(" << to << ", " << size
	         << ", " << (void *)from << ")" << endl;
    debug_stream.unlock();
    
#endif

    struct RegionHead *q = (struct RegionHead *) (segVaddr);

    if (segVaddr == 0)
		return -1;

    if (to + size > q->regionSize)
		return -1;

    if ((to < 0) || (size < 0))
		return -1;
    
    ::memcpy(to + segVaddr + OVERHEAD, from, (int) size);
    isNew = FALSE;

	FlushViewOfFile(to + segVaddr + OVERHEAD, size);

    return size;
}

#ifndef WIN32

int SharedSegment::shmid () const
{
    return segId;
}

#else

const char* SharedSegment::shmid () const
{
	return segId;
}

#endif

/*
 * Protected operations
 */

/*
 * Constructor - This one does no explicit initialisation of the area -
 * that job is the perogative of the derived class (by calling smInit).
 */

SharedSegment::SharedSegment ()
                             : segId(0),
#ifdef WIN32
							   fileName(0),
#endif
					  		   segVaddr(0),
							   isNew(FALSE),
							   lastError()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_SHMEM << VIS_PROTECTED;
    debug_stream << "SharedSegment::SharedSegment()" << endl;
    debug_stream.unlock();
    
#endif
}

/*
 * Return segment virtual address (adjusting for segment header).
 */

char *SharedSegment::getSegVaddr () const
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

#ifndef WIN32

char *SharedSegment::smInit ( const key_t smKey, size_t smSize )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_SHMEM << VIS_PRIVATE;
    debug_stream << "SharedSegment::smInit(" << smKey << ", " 
		 << smSize << ")" << endl;
    debug_stream.unlock();
    
#endif
    
    char *start = 0;
    struct RegionHead *q = 0;
    Boolean initialised = TRUE;
    long size = smSize + OVERHEAD;

    /*
     * Try for exclusive creation of segment. If this fails see if this
     * was because the segment already existed. If so attach to it.
     * Otherwise there's an error.
     */

    isNew = FALSE;

    if ((segId = ::shmget(smKey, (int) size, 0666 | IPC_CREAT | IPC_EXCL)) != -1)
    {
        initialised = FALSE;
    }
    else
    {
        int savedErrno = errno;

        if ((errno != EEXIST) ||
	    ((segId = ::shmget(smKey, (int) size, 0666)) == -1))
	{
	    char buff[256];
	    ostrstream temp(buff, 256);
	    Error *error;

	    clearErrorList();

	    temp << "SharedSegment::SharedSegment() failed: ";
            if (savedErrno != EEXIST)
		temp << " Could not create segment ";
	    else
	        temp << " Could not attach segment ";

	    temp << "with key: " << smKey << " errno: " << errno << ends;

	    error = new Error(FAC_SHMEM, SHM_CREATE_FAILED, buff);

	    addError(error);
            return 0;
        }
    
    }

    /* We now have a valid shared mem id - attach and initialise if needed */

    if ((start = attach(segId)) == 0)
    {
	Error *error;

        clearErrorList();
	error = new Error(FAC_SHMEM, SHM_ATTACH_FAILED,
			  "SharedSegment::attach() failed");

	addError(error);
	return 0;
    }
    
    q = (RegionHead *) start;
    if (!initialised)
    {
	/* clear all of allocated memory and fill in control info */
	
	isNew = TRUE;
	::memset(start, '\0', (int) size);
	q->regionKey = smKey;
	q->regionSize = smSize;
	segVaddr = start;
    }
    else				 /* check info supplied matches */
					 /* that already there */
    {
	if (q->regionSize != smSize)
	{
	    Error *error;

        clearErrorList();
	    error = new Error(FAC_SHMEM, SHM_SIZE_MISMATCH, "SharedSegment::SharedSegment() : Segment size mismatch");

	    addError(error);

	    detach(segId);		/* detach segment if wrong size */
	    return 0;
	}
    }
    
    return start;
}

#else

char *SharedSegment::smInit ( const key_t smKey, size_t smSize )
{
    char *start = 0;
    struct RegionHead *q = 0;
    Boolean initialised = FALSE;
    DWORD size = smSize + OVERHEAD;
	Uid u;

	/*
	 * Get this via getenv.
	 */

	fileName = ::new char[MAX_PATH];
	::memset(fileName, '\0', MAX_PATH);

	if (smKey == -1)
	{
		char uidBuff[MAX_PATH+1];
		ostrstream os(uidBuff, MAX_PATH+1, ios::out);

		os << u << ends;

		sprintf(fileName, "c:/temp/SegmentFile_%s", uidBuff);
	}
	else	
		sprintf(fileName, "c:/temp/SegmentFile_%d", smKey);

    /*
     * Try for exclusive creation of segment. If this fails see if this
     * was because the segment already existed. If so attach to it.
     * Otherwise there's an error.
     */

    isNew = FALSE;

	/*
	 * Attempt to create the file to map on. If it already exists then create it and
	 * go straight to the map stage. For protection reasons we create the file as a
	 * hidden file.
	 */

	if ((fileHandle = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
								0, OPEN_ALWAYS, 0, 0)) == INVALID_HANDLE_VALUE)
	{
		error_stream << WARNING << "SharedSegment CreateFile " << fileName << " failed." << endl;
		return 0;
	}

	/*
	 * Have just created the file, so write its size.
	 */

	if (GetLastError() != ERROR_ALREADY_EXISTS)
	{
		char* buffer = ::new char[size];
		DWORD written = 0;

		if (!WriteFile(fileHandle, buffer, size, &written, NULL))
		{
			::delete [] buffer;

			CloseHandle(fileHandle);
			return 0;
		}

		::delete [] buffer;
	}

	/*
	 * File is opened and/or created.
	 */

	segId = ::new char[MAX_PATH];
	::memset(segId, '\0', MAX_PATH);

	if (smKey == -1)
	{
		char uidBuff[MAX_PATH+1];
		ostrstream os(uidBuff, MAX_PATH+1, ios::out);

		os << u << ends;

		sprintf(segId, "c:/temp/MappingFile_%s", uidBuff);
	}
	else	
		sprintf(segId, "c:/temp/MappingFile_%d", smKey);

	/*
	 * This will either create a new file mapping object, or bind us to an already
	 * existing one.
	 */

	segHandle = CreateFileMapping(fileHandle, NULL, PAGE_READWRITE, 0, 0, segId);

	if (segHandle == NULL)
	{
		char buff[256];
		ostrstream temp(buff, 256);
		Error *error;

		clearErrorList();

		temp << "SharedSegment::SharedSegment() CreateFileMapping failed: ";
		temp << "with key: " << smKey << " errno: " << GetLastError() << ends;

		error = new Error(FAC_SHMEM, SHM_CREATE_FAILED, buff);

		addError(error);

		return 0;
	}

	if (GetLastError() == ERROR_ALREADY_EXISTS)  // was already created.
		initialised = TRUE;

    /* We now have a valid shared mem id - attach and initialise if needed */

    if ((start = attach(segHandle)) == 0)
    {
		Error *error;

        clearErrorList();
		error = new Error(FAC_SHMEM, SHM_ATTACH_FAILED, "SharedSegment::attach() failed");

		addError(error);
		return 0;
    }
    
    q = (RegionHead *) start;

    if (!initialised)
    {
		/* clear all of allocated memory and fill in control info */
		
		isNew = TRUE;
		::memset(start, '\0', size);
		q->regionSize = smSize;
		segVaddr = start;
    }
    else				 /* check info supplied matches */
						/* that already there */
    {
		if (q->regionSize != smSize)
		{
			Error *error;

			clearErrorList();
			error = new Error(FAC_SHMEM, SHM_SIZE_MISMATCH,
					  "SharedSegment::SharedSegment() : Segment size mismatch");

			addError(error);

			detach(segHandle);		/* detach segment if wrong size */
			return 0;
		}
    }
    
    return start;
}

#endif

Boolean SharedSegment::IsNew () const 
{
    return isNew;
}

/*
 * Attaching to an existing segment. If we have attached the segment
 * already in this process it is in the segment mapping list. If so
 * we update the usage count and return the virtual address found there.
 * If not found we do a shmat and build a new entry.
 */

#ifndef WIN32

char *SharedSegment::attach ( int shmId )
{
    SegMap *p = SharedSegment::segMapPtr;

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

		char *start;

		if ((start = (char*)::shmat(shmId, 0, 0)) != (char *) -1)
		{
			/* successful shmat - add to mapping table */

			p = new SegMap;

			if (p)
			{
				p->usage = 0;
				p->addr = start;
				p->id = shmId;
				p->link = SharedSegment::segMapPtr;
				SharedSegment::segMapPtr = p;
			}
		}
		else
		{
			int savedErrno = errno;
			char buff[256];
			ostrstream temp(buff, 256);
			Error *error = 0;

			clearErrorList();

			temp << "SharedSegment::attach() failed for segment with id: ";
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
		return 0;
}

/*
 * Detach a segment. Update the usage count in the mapping table
 * and if it drops to zero check the usage count on the segment
 * itself. If that is zero actually remove the segment from the system.
 */

void SharedSegment::detach ( int shmId )
{
    SegMap *p = SharedSegment::segMapPtr;
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
			/* Usage by this process is now zero check if should remove from system */

			if (p->addr != 0)
			{
				struct shmid_ds ds;
				
				/* Keep purify happy... */
				ds.shm_nattch = 0;

				::shmdt(p->addr);
				::shmctl(shmId, IPC_STAT, &ds);
				if (ds.shm_nattch == 0)
					::shmctl(shmId, IPC_RMID, &ds);
			}

			/* Thow away map entry */

			if (p == SharedSegment::segMapPtr)
				SharedSegment::segMapPtr = p->link;
			else
			{
				q->link = p->link;
			}

			delete p;
		}
    }
}

#else

char *SharedSegment::attach ( HANDLE shmId )
{
    SegMap *p = SharedSegment::segMapPtr;

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

		char *start;

		if ((start = (char*) MapViewOfFile(shmId, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL)
		{
			/* successful shmat - add to mapping table */

			p = new SegMap;

			if (p)
			{
				p->usage = 0;
				p->addr = start;
				p->id = shmId;
				p->link = SharedSegment::segMapPtr;
				SharedSegment::segMapPtr = p;
			}
		}
		else
		{
			char buff[256];
			ostrstream temp(buff, 256);
			Error *error = 0;

			clearErrorList();

			temp << "SharedSegment::attach() failed for segment with id: ";
			temp << shmId << " errno: " << GetLastError() << ends;

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
		return 0;
}

/*
 * Detach a segment. Update the usage count in the mapping table
 * and if it drops to zero check the usage count on the segment
 * itself. If that is zero actually remove the segment from the system.
 */

void SharedSegment::detach ( HANDLE shmId )
{
    SegMap *p = SharedSegment::segMapPtr;
    SegMap *q = p;

    /* Scan existing list for id */

    while ( p != 0 )
    {
		if (p->id == shmId)
			break;

		q = p;
		p = p->link;
    }

	cout << "detaching " << fileName << endl;
	cout << "count " << p->usage << endl;

    if (p != 0)
    {
		/* found map entry - update it */

		p->usage--;
		if (p->usage == 0)
		{
			/* Usage by this process is now zero check if should remove from system */

			if (!UnmapViewOfFile(p->addr))
				error_stream << WARNING
				<< "SharedSegment::detach - UnmapViewOfFile failed with errno " << GetLastError() << endl;

			if (!CloseHandle(segHandle))
				cout << "segHandle close error" << endl;

			if (!CloseHandle(fileHandle))
				cout << "fileHandle close error" << endl;

			cout << "deleting " << segId << endl;

			if (!DeleteFile(segId))
				cout << "delete " << segId << " error" << endl;

			cout << "deleting " << fileName << endl;

			if (!DeleteFile(fileName))
				cout << "delete " << fileName << " error" << endl;

			/* Throw away map entry */

			if (p == SharedSegment::segMapPtr)
				SharedSegment::segMapPtr = p->link;
			else
			{
				q->link = p->link;
			}

			delete p;
		}
    }
}

#endif

#ifdef WIN32

/*
 * We can only return this processes usage count, and not the total on the shared
 * segment.
 */

int SharedSegment::usage () const
{    
    SegMap *p = SharedSegment::segMapPtr;
    int myUsage = 0;

    /* Scan existing list for id */

    while ( p != 0 )
    {
		if (p->id == segId)
		{
			myUsage = p->usage - 1;	    
			break;
		}

		p = p->link;
    }
	
	return myUsage;
}

#endif

#ifdef NO_INLINES
#  define SHAREDSEG_CC_
#  include "SharedSeg.n"
#  undef SHAREDSEG_CC_
#endif
