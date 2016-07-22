/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NameInfoDriver.cc
 */

#ifndef DEBUG_H_
#include <Common/Debug.h>
#endif

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#ifndef ERROR_H_
#include <Common/Error.h>
#endif

#ifndef UNISTD_H_
#include <System/unistd.h>
#endif

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef ERRNO_H_
#include <System/errno.h>
#endif

#ifndef STRSTREAM_H_
#include <System/strstream.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef SYS_PARAM_H_
#include <System/sys/param.h>
#endif

#ifndef ARPA_INET_H_
#  include <System/arpa/inet.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

#include <RPC/ArjServers/NameInfo.h>

const int uid_Size = 80;
const int host_Size = 400;


unsigned long hostInetAddr ()
{
    static unsigned long myAddr;
    static char hostname[MAXHOSTNAMELEN];

    struct hostent *h;
    
    ::gethostname(hostname, MAXHOSTNAMELEN);
    h = ::gethostbyname(hostname);
    ::memcpy(&myAddr, h->h_addr, h->h_length);

    return myAddr;
}

int main(int argc, char** argv)
{
    Boolean finished = FALSE;
    Boolean done = FALSE;
    Boolean newDatabase = FALSE;
    NameInfo* X;
    unsigned long hostAddr = hostInetAddr();
    struct in_addr inet;

    inet.s_addr = hostAddr;
    char* name = inet_ntoa(inet);
    const char* n_name = ":0:0:0";
    char* s_uid = new char[strlen(name) + strlen(n_name) +1];
    memset(s_uid, '\0', strlen(name) + strlen(n_name) +1);
    memcpy(s_uid, name, strlen(name));
    memcpy(s_uid+strlen(name), n_name, strlen(n_name));
    Uid Y(s_uid);

    if (argc == 2)
    {
	if (strcmp(argv[1], "-new") == 0)
	{
	    X = new NameInfo(done);
	    newDatabase = TRUE;
	}
    }
    else
    {
	X = new NameInfo(Y, done);
    }

    if (!done)
    {
	cout << "Error while creating NameInfo object\n" << flush;
	if (X)
	    delete X;
	exit(0);
    }

    while (!finished)
    {
	char opt;
	
	cout << "1: Get Replication Information\n" << flush;
	cout << "2: Set Replication Information\n" << flush;
	cout << "3: Quit\n" << flush;

	cin >> opt;

	switch (opt)
	{
	case '1': {
	    AtomicAction B;
	    Boolean result = FALSE;
	    NameData *marker, *ptr;
	    int indx = 1, level = 0;

	    B.Begin();

	    marker = X->GetReplicationInfo(result, level);

	    if (!result)
	    {
		cout << "Error while attempting to get information\n" << flush;
		B.Abort();
		break;
	    }

	    ptr = marker;
	    while (ptr != 0)
	    {
		cout << "Entry " << indx << " has <" << ptr->hostname << ", " << *ptr->myUID << ">\n" << flush;
		ptr = ptr->next;
		indx++;
	    }

	    if (B.End() != COMMITTED)
		cout << "Error while committing action\n" << flush;

	    break;
	}
	case '2': {
	    AtomicAction B;
	    Boolean result = FALSE;
	    NameData *marker, *ptr;
	    int indx = 0;

	    cout << "How many data entries?\n" << flush;
	    cin >> indx;
	    if (indx < 1)
	    {
		cout << "Number must be positive\n" << flush;
		break;
	    }

	    marker = new NameData;
	    ptr = marker;
	    for (int i = 1; i <= indx; i++)
	    {
		cout << "What is the hostname?\n" << flush;
		ptr->hostname = new char[host_Size];
		::memset(ptr->hostname, '\0', host_Size);
		cin >> ptr->hostname;

		cout << "What is the UID?\n" << flush;
		char u_id[uid_Size];
		memset(u_id, '\0', uid_Size);
		cin >> u_id;
		ptr->myUID = new Uid(u_id);
		
		if (i+1 <= indx)
		{
		    ptr->next = new NameData;
		    ptr = ptr->next;
		}
	    }

	    B.Begin();

	    result = X->SetReplicationInfo(marker);

	    if (!result)
	    {
		cout << "Error while attempting to set information\n" << flush;
		B.Abort();
		break;
	    }

	    cout << "Information set ok\n" << flush;
	    if (B.End() != COMMITTED)
		cout << "Error while committing action\n" << flush;

	    break;
	}
	case '3': {
	    finished = TRUE;
	    break;
	}
	default:
	    break;
        }		   
    }

    if (newDatabase)
    {
	const char* dbName = X->type();
	Buffer touse;
	struct in_addr Z;
	unsigned long hostAddr;
	unsigned long cpu;
	unsigned long sec;
	unsigned long usec;

	X->get_uid().pack(touse);
	touse.unpack(hostAddr);
	touse.unpack(cpu);
	touse.unpack(sec);
	touse.unpack(usec);
	Z.s_addr = hostAddr;

	char* dirName = new char[strlen(OBJECTSTOREDIR_Q)+strlen(dbName)+1];
	memset(dirName, '\0', strlen(OBJECTSTOREDIR_Q)+strlen(dbName)+1);
	memcpy(dirName, OBJECTSTOREDIR_Q, strlen(OBJECTSTOREDIR_Q));
	memcpy(dirName+strlen(OBJECTSTOREDIR_Q), dbName, strlen(dbName));
	if (chdir(dirName))
	    cout << "Error. Could not chdir to " << dirName << "\n" << flush;
	else
	{
	    char* oldName = new char[31];
	    memset(oldName, '\0', 31);

	    ostrstream oss(oldName, 30, ios::out);
	    oss << inet_ntoa(Z) << ":";

	    oss.setf(ios::dec);
	    oss << dec << cpu;
	    oss << ":";
	    oss << dec << sec;
	    oss << ":";
	    oss << dec << usec << endl;

	    if (rename(oldName, s_uid) != 0)
		cout << "Error " << errno << " while attempting to rename object state\n" << flush;
	}
    }

    if (X)
	delete X;
}

