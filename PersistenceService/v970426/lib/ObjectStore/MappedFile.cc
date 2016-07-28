/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MappedFile.cc,v 1.8 1995/09/11 12:23:04 ngdp Exp $
 */

#ifndef STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <os/unistd.h>
#endif

#ifndef FCNTL_H_
#  include <os/fcntl.h>
#endif

#ifndef SYS_STAT_H_
#  include <os/sys/stat.h>
#endif

#ifndef SYS_MMAN_H_
#  include <os/sys/mman.h>
#endif

#ifndef SIGNAL_H_
#  include <os/signal.h>
#endif

#ifndef ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef PERSISTENCE_SERVICE

#ifndef PROTOS_H_
#  include <os/protos.h>
#endif

#endif

#ifndef STRSTREAM_H_
#  include <os/strstream.h>
#endif

#ifndef STRING_H_
#  include <os/string.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef OBJSTATE_H_
#  include <Common/ObjState.h>
#endif

#ifndef MAPPEDFILE_H_
#  include <ObjectStore/MappedFile.h>
#endif

#ifndef SEMAPHORE_H_
#  include <Common/Semaphore.h>
#endif

SIG_ARGTYPE oldSegvHandler = 0;

Boolean MappedFile::signalSeen = FALSE;

#ifdef HAVE_INT_SIG
int sigbusHandler (int DOTDOTDOT)
#else
void sigbusHandler (int DOTDOTDOT)
#endif
{
    /* re-establish signal handlers */
    ::signal(SIGBUS, sigbusHandler);

    MappedFile::signalSeen = TRUE;
}

#ifdef HAVE_INT_SIG
int segvHandler (int DOTDOTDOT)
#else
void segvHandler (int DOTDOTDOT)
#endif
{
    /* re-establish signal handlers */
    ::signal(SIGSEGV, segvHandler);

    MappedFile::signalSeen = TRUE;
}

void establishSegvHandler ()
{
    if (oldSegvHandler == 0)
	oldSegvHandler = ::signal(SIGSEGV, segvHandler);
}

void restoreSegvHandler ()
{
    ::signal(SIGSEGV,oldSegvHandler);
    oldSegvHandler = 0;
}

MappedFile::MappedFile ( const char *rootPath, off_t initialSize )
		       : currentSize(0),
			 fd(-1),
			 mappedMemory(0)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "MappedFile::MappedFile ( " << rootPath << ", " 
                 << initialSize << " )" << endl;
    debug_stream.unlock();
    
#endif

#ifdef SYSV
    pageSize = sysconf(_SC_PAGESIZE);
#else
    pageSize = getpagesize();
#endif
    
    signal(SIGBUS, sigbusHandler);
    
    if ((fd = open(rootPath, O_RDWR | O_CREAT, 0666)) != -1)
    {
	sizeUp(initialSize);
    }
}

MappedFile::~MappedFile ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "MappedFile::~MappedFile ()\n" << flush;
    debug_stream.unlock();
    
#endif

    sync();

#ifdef HAVE_MMAP
    if (mappedMemory != 0)
	munmap(mappedMemory, currentSize);
#endif
    
    if (fd != -1)
	close(fd);
}

Boolean MappedFile::isValid () const
{
    return mappedMemory != 0;
}

Boolean MappedFile::get ( char *out, off_t from, size_t size )
{
    if ((mappedMemory) && sizeUp(from+size))
    {
	establishSegvHandler();
	memcpy(out, mappedMemory + from, size);
	restoreSegvHandler();
	
	if (signalSeen)
	{
	    signalSeen = FALSE;
#ifdef HAVE_MMAP
	    munmap(mappedMemory,currentSize);
#endif
	    mappedMemory = 0;
	}
	else
	    return TRUE;
    }
    
    return FALSE;
}

Boolean MappedFile::put ( const char *in, off_t from, size_t size )
{
    if ((mappedMemory) && sizeUp(from + size))
    {
	establishSegvHandler();
	memcpy(mappedMemory + from, in, size);
	restoreSegvHandler();
	
	if (signalSeen)
	{
	    signalSeen = FALSE;
#ifdef HAVE_MMAP
	    munmap(mappedMemory,currentSize);
#endif
	    mappedMemory = 0;
	}
	else
	    return TRUE;
    }
    
    return FALSE;
}

Boolean MappedFile::sizeUp ( off_t size )
{
    off_t newSize = size + sizeof(off_t);
#ifdef HAVE_MMAP
    struct stat statBuf;

    if (pageSize == 0)
	return FALSE;
    
    /* round up to nearest multiple of pageSize */

    if (newSize % pageSize)
	newSize += pageSize - (newSize % pageSize);

    /* Need to extend/init our mapping */
    
    /* Determine current size of underlying file */
	
    if (fstat(fd, &statBuf) == 0)
    {
	if (statBuf.st_size < newSize)
	{
	    /* Too small - enlarge it by writing at requested offset */
	    if ((lseek(fd, newSize-sizeof(newSize), SEEK_SET) != (newSize-sizeof(newSize))) ||
		(write(fd, (char *)&newSize, sizeof(newSize)) != sizeof(newSize)))
	    {
		if (mappedMemory)
		{
		    munmap(mappedMemory, currentSize);
		    mappedMemory = 0;
		}
		
		return FALSE;	/* failed to extend */
	    }
	}
	else
	    newSize = statBuf.st_size;
    }
    else 
	return FALSE;

    if (currentSize < newSize)
    {
	munmap(mappedMemory, currentSize);
	
	if ((mappedMemory = mmap(0, newSize, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
	    currentSize = 0;
	    mappedMemory = 0;
	}
	else
	    currentSize = newSize;
    }

    if (mappedMemory != 0)
      return TRUE;
    else
      return FALSE;
    
#else
    return FALSE;
#endif
}

Boolean MappedFile::sync ()
{
#ifdef HAVE_MMAP
    if (mappedMemory)
    {
	if (msync(mappedMemory, currentSize, MS_SYNC) == 0)
	    return TRUE;
    }
#endif
    return FALSE;
}

Boolean MappedFile::truncate ( off_t newSize )
{
    if ((fd != -1) && (ftruncate(fd, newSize) == 0))
	return TRUE;

    return FALSE;
}
