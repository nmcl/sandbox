/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <System/iostream.h>
#include <System/unistd.h>
#include <System/limits.h>
#include <System/netdb.h>
#include <System/stdlib.h>
#include <System/sys/socket.h>
#include <System/netinet/in.h>
#include <System/sys/param.h>
#include <System/string.h>
#include <System/signal.h>
#include <System/strstream.h>

#include <Arjuna/AtomicA.h>
#include <Thread/Thread.h>

#include <PortAddress.h>
#include <TCPConnection.h>
#include <RelPort.h>

#include <Template/TSHashList.h>

#ifndef OBJECTDATA_H_
#  include "ObjectData.h"
#endif

ThreadSafeHashList<AtomicAction> actions(11);
ThreadSafeHashList<ObjectData> objects(11);
Boolean doDebug = FALSE;

enum Opcode { CREATE = 0, DESTROY, GET, SET, BEGIN, COMMIT, ABORT };

static const int poolSize = 20;

Thread* threadPool[poolSize];
int numberOfThreads = 0;

char* uidToString (const Uid& u)
{
    char uidString[128];  // no Uid is larger than 128 characters

    ::memset(uidString, '\0', 128);

    ostrstream output(uidString, 128);

    output << u;

    char* returnString = ::new char[::strlen(uidString)+1];
    ::strcpy(returnString, uidString);
    
    return returnString;
}

void signal_handler (int dummy DOTDOTDOT)
{
    for (int j = 0; j < poolSize; j++)
    {
	if (threadPool[j])
	{
	    delete threadPool[j];
	    threadPool[j] = 0;
	}
    }

    ObjectData* ptr = objects.orderedPop();

    while (ptr)
    {
	delete ptr;
	ptr = objects.orderedPop();
    }

    AtomicAction* act = actions.orderedPop();

    while (act)
    {
	delete act;
	act = actions.orderedPop();
    }

    exit(0);
}

ObjectData* createObject (const Uid& u)
{
    ObjectData* ptr = objects.lookFor(u);

    if (!ptr)
    {
	int done = -1;

	ptr = new ObjectData(u, done);

	if (done == 0)
	    objects.insert(ptr);
	else
	{
	    delete ptr;
	    ptr = 0;
	}
    }

    return ptr;
}

Boolean destroyObject (const Uid& u)
{
    if (numberOfThreads == 1)
    {
	ObjectData* ptr = objects.remove(u);

	if (ptr)
	{
	    delete ptr;
	    return TRUE;
	}
    }
    
    return FALSE;
}

/*
 * Each operation is performed as a separate top-level atomic action.
 */

void* doWork (void* param)
{
    if (!param)
    {
	cerr << "StateDriver thread has no work!" << endl;
	
	return 0;
    }

    numberOfThreads++;
    
    /*
     * Get work and deal with it. Then go back to receive
     * further work.
     */

    Buffer request;
    Buffer reply;
    Buffer* work[1];
    Buffer* result[1];
    int size = 1;
    int tmpVal = 0;
    TCPConnection* connection = (TCPConnection*) param;
    
    work[0] = &request;
    result[0] = &reply;

    connection->receiveMessage(work, size);

    Thread::yield();

    Boolean res = request.unpack(tmpVal);

    if (!res)
    {
	cout << "\nStateDriver received an illegal message." << endl;
    }
    else
    {
	Opcode opcode = (Opcode) tmpVal;

	if (doDebug)
	    cout << "\nStateDriver received request. Processing ... ";

	switch (opcode)
	{
	case CREATE:
	    {
		if (doDebug)
		    cout << "Received CREATE operation." << endl;

		int done = -1;
		ObjectData* obj = new ObjectData(done);

		if (done == 0)
		{
		    char* id = uidToString(obj->get_uid());

		    reply.pack(TRUE);
		    reply.packNonMappedString(id);

		    ::delete [] id;

		    objects.insert(obj);
		    
		    break;
		}
		else
		    delete obj;

		reply.pack(FALSE);
	    }
	break;
	case GET:
	    {
		if (doDebug)
		    cout << "Received GET operation." << endl;

		char* id = 0;
		char* tranID = 0;
		
		if (request.unpack(tranID) && request.unpack(id))
		{
		    if (doDebug)
		    {
			cout << "Object ID: " << id << endl;
			cout << "Transaction: " << tranID << endl;
		    }
		    
		    Uid u(id, TRUE);
		    Uid tran(tranID, TRUE);

		    if ((u != NIL_UID) && (tran != NIL_UID))
		    {
			AtomicAction* A = actions.lookFor(tran);

			if (A != NULL)
			{
			    int stateSize = 0;
			    ObjectData* obj = createObject(u);

			    if (obj != NULL)
			    {
				Thread::current()->pushAction(A);

				void* state = obj->get(stateSize);

				reply.pack(TRUE);
				reply.packNonMappedBlock(state, stateSize);

				if (state)
				    delete state;

				if (tranID)
				    ::delete [] tranID;
				
				if (id)
				    ::delete [] id;

				break;
			    }
			}
		    }
		}

		reply.pack(FALSE);

		if (tranID)
		    ::delete [] tranID;

		if (id)
		    ::delete [] id;
	    }
	break;
	case SET:
	    {
		if (doDebug)
		    cout << "Received SET operation." << endl;

		char* id = 0;
		char* tranID = 0;
		void* state = 0;
		size_t stateSize = 0;

		if (request.unpack(tranID) && request.unpack(id) && request.unpack(state, stateSize))
		{
		    if (doDebug)
		    {
			cout << "Object ID: " << id << endl;
			cout << "Transaction: " << tranID << endl;
		    }
		    
		    Uid u(id, TRUE);
		    Uid tran(tranID, TRUE);

		    if ((u != NIL_UID) && (tran != NIL_UID))
		    {
			AtomicAction* A = actions.lookFor(tran);

			if (A != NULL)
			{
			    ObjectData* obj = createObject(u);

			    if (obj != NULL)
			    {
				Thread::current()->pushAction(A);
				
				if (obj->set(state, stateSize))
				{
				    reply.pack(TRUE);

				    if (tranID)
					::delete [] tranID;
				    
				    if (id)
					::delete [] id;

				    if (state)
					delete state;

				    break;
				}
			    }
			}
		    }
		}

		if (tranID)
		    ::delete [] tranID;

		if (id)
		    ::delete [] id;

		if (state)
		    delete state;
		
		reply.pack(FALSE);
	    }
	break;
	case BEGIN:
	    {
		if (doDebug)
		    cout << "Received BEGIN operation." << endl;

		AtomicAction* A = new AtomicAction();

		if (actions.insert(A))
		{
		    A->begin();

		    char* uidString = uidToString(A->get_uid());

		    reply.pack(TRUE);
		    reply.packNonMappedString(uidString);

		    ::delete [] uidString;

		    break;
		}
		else
		    delete A;

		reply.pack(FALSE);
	    }
	break;
	case COMMIT:
	    {
		if (doDebug)
		    cout << "Received COMMIT operation." << endl;

		char* uidString;

		if (request.unpack(uidString))
		{
		    if (doDebug)
			cout << "Transaction: " << uidString << endl;
		    
		    Uid u(uidString, TRUE);

		    ::delete [] uidString;
		    
		    if (u != NIL_UID)
		    {
			AtomicAction* A = actions.remove(u);

			if (A != NULL)
			{
			    Boolean result = FALSE;
			    
			    Thread::current()->pushAction(A);
			    
			    if (A->commit() == COMMITTED)
			    {
				reply.pack(TRUE);
				result = TRUE;
			    }

			    Thread::current()->popAction();
			    
			    delete A;

			    if (result)
				break;
			}
		    }
		}

		reply.pack(FALSE);
	    }
	break;
	case ABORT:
	    {
		if (doDebug)
		    cout << "Received ABORT operation." << endl;

		char* uidString;

		if (request.unpack(uidString))
		{
		    if (doDebug)
			cout << "Transaction: " << uidString << endl;
		    
		    Uid u(uidString, TRUE);

		    ::delete [] uidString;
		    
		    if (u != NIL_UID)
		    {
			AtomicAction* A = actions.remove(u);

			if (A != NULL)
			{
			    Boolean result = FALSE;
			    
			    Thread::current()->pushAction(A);
			    
			    if (A->abort() == ABORTED)
				result = TRUE;

			    Thread::current()->popAction();
			    
			    delete A;

			    if (result)
				break;
			}
		    }
		}

		reply.pack(FALSE);
	    }
	break;
	case DESTROY:
	    {
		if (doDebug)
		    cout << "Received DESTROY operation." << endl;

		char* uidString;

		if (request.unpack(uidString))
		{
		    if (doDebug)
			cout << "Object ID: " << uidString << endl;

		    Uid u(uidString, TRUE);

		    ::delete [] uidString;
		    
		    if (u != NIL_UID)
		    {
			reply.pack(destroyObject(u));
			break;
		    }
		}

		reply.pack(FALSE);
	    }
	break;	
	default:
	    cerr << "Opcode error." << endl;
	    reply.pack(FALSE);
	    
	    break;
	}

	if (connection->sendMessage(result, 1) != TCPConnection::DONE)
	    cerr << "StateDriver error - could not send reply." << endl;
    }

    delete connection;

    numberOfThreads--;
    
    return 0;
}

void usage (char* name)
{
    cout << "Usage: " << name << " [-port <port>] [-debug] [-help]" << endl;
}

int main (int argc, char** argv)
{
    int portNumber = 0;

    for (int i = 1; i < argc; i++)
    {
	if (::strcmp(argv[i], "-port") == 0)
	{
	    portNumber = ::atoi(argv[i+1]);
	    i++;
	}
	else
	{
	    if (::strcmp(argv[i], "-help") == 0)
	    {
		usage(argv[0]);
		return 0;
	    }
	    else
	    {
		if (::strcmp(argv[i], "-debug") == 0)
		    doDebug = TRUE;
		else
		{
		    cout << "Unknown option " << argv[i] << endl;
		    usage(argv[0]);

		    return -1;
		}
	    }
	}
    }
    
    ::signal(SIGINT, signal_handler);
    ::signal(SIGQUIT, signal_handler);
    ::signal(SIGTERM, signal_handler);

    for (i = 0; i < poolSize; i++)
    {
	threadPool[i] = 0;
    }
    
    PortAddress addr(portNumber), from;
    ReliablePort port(addr);

    if (portNumber == 0)
    {
	unsigned short p = 0;
	PortAddress tmpAddr;

	port.getAddress(tmpAddr);

	tmpAddr.getPort(p);

	portNumber = p;
    }
    
    cout << "ObjectStore listening on port " << portNumber << endl;
    
    for (;;)
    {
	/*
	 * Find a spare slot in the thread pool before accepting any connection.
	 */
	
	int index = 0;
	Boolean found = FALSE;
	
	for (index = 0; (index < poolSize) && !found; index++)
	{
	    if (threadPool[index] == 0)
	    {
		found = TRUE;
		break;
	    }
	    else
	    {
		if (threadPool[index]->status() == Thread::THR_IS_TERMINATED)
		{
		    delete threadPool[index];
		    threadPool[index] = 0;
		    found = TRUE;
		    break;
		}
	    }
	}

	if (found)
	{
	    TCPConnection* connection = port.acceptConnection();
	    Thread* t = new Thread(doWork, connection);

	    threadPool[index] = t;
	    
	    t->resume();
	}
	else
	    cerr << "WARNING: thread pool exhausted." << endl;

	Thread::yield();
    }

    for (int j = 0; j < poolSize; j++)
    {
	if (threadPool[j])
	{
	    delete threadPool[j];
	    threadPool[j] = 0;
	}
    }

    return 0;
}
