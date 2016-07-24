/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $id:$
 */

#define TRANSPORTER_SERVER

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef FSTREAM_H_
#include <System/fstream.h>
#endif

#ifndef MEMORY_H_
#include <System/memory.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef DEBUG_H_
#include <Common/Debug.h>
#endif

#ifndef COMMON_H_
#include "common.h"
#endif

#include "Transporter_stub.h"

/* This class is the workhorse of the interface logging facility. When an interface has
 * been selected by an application programmer the interface browser contacts the
 * Transporter on the node on which the interface resides and asks it to pass the interface
 * description back to it. If the interface description cannot be found then an error occurs.
 * Because the Transporter is a stateless entity it does not need to be derived from LockManager
 * or StateManager.
 */

Transporter::Transporter (int &res)
{
    _arjuna_debug->set_debuglevel(FULL_DEBUGGING);

#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Transporter::Transporter(int &res)\n" << flush;
#endif

    res = 1;
}

Transporter::~Transporter ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Transporter::~Transporter()\n" << flush;
#endif
}

/* Given the pathname of the required interface description (packed in the Buffer 'data')
 * the Transporter will load the interface and pack it into the return buffer.
 */

Buffer Transporter::Get (Buffer data, int& res)
{
    int i = 0;
    char c, contents[filesize];
    char* pathname;
    Buffer buff;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Buffer Transporter::Get(Buffer data, int& res)\n" << flush;
#endif

    memset(contents, '\0', filesize);
    data.unpack(pathname);
    ifstream inFile(pathname, ios::in);

    if (inFile)
    {
	res = 1;
	while ((inFile.get(c)) && (i < filesize))
	{
	    contents[i] = c;
	    i++;
	}

	res = 0;

	buff.pack(contents);
	return buff;
    }
    else
    {
	cerr << "Error: no such file as " << pathname << "\n";
	return buff;
    }
}
