/*
 * Copyright 1990, 1991, 1992,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 *
 */

/*
 * $Id: ServerDispatcher.cc,v 1.2 1993/10/22 11:50:49 nsmw Exp $
 */


#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_UIO_H_
#  include <System/sys/uio.h>
#endif

#ifndef SERVERDISPATCHER_H_
#  include <StubGen/ServerDispatcher.h>
#endif


DispatcherList::DispatcherList (Dispatcher* toAdd)
			       : _dispatcher(toAdd),
				 _next(0)
{
}

DispatcherList::~DispatcherList ()
{
    if (_dispatcher)
	delete _dispatcher;
    if (_next)
	delete _next;
}


ServerDispatcher::ServerDispatcher ()
				   : headOfCreateList(0),
				     headOfObjectList(0)
{
}

ServerDispatcher::~ServerDispatcher ()
{
    if (headOfCreateList)
	delete headOfCreateList;
    if (headOfObjectList)
	delete headOfObjectList;
}

Dispatcher::DispatcherStatus ServerDispatcher::addDispatcher (Dispatcher* toAdd)
{
    if (toAdd)
    {
	DispatcherList* element = new DispatcherList(toAdd);
	element->_next = headOfCreateList;
	headOfCreateList = element;

	return Dispatcher::DONE;
    }
    
    return Dispatcher::NOTDONE;
}

int ServerDispatcher::initialise (int argc, char** argv)
{
    return _rpcHandle.Initialise(argc, argv);
}

Dispatcher::DispatcherStatus ServerDispatcher::end ()
{
    return DONE;
}

Dispatcher::DispatcherStatus ServerDispatcher::begin ()
{
    for (;;)
    {
	Buffer* work[MAXBUFFERS];
	int workSize = 0;

	for (int i = 0; i < MAXBUFFERS; i++)
	    work[i] = (Buffer*) 0;
	
	if (_rpcHandle.GetWork(work, workSize) == SimpleRpcServer::DONE)
	{
	    Dispatcher::DispatcherStatus status = callDispatcher(work, workSize);

	    for (int i = 0; i < workSize; i++)
	    {
		if (work[i])
		    delete work[i];
	    }

	    if (status != Dispatcher::DONE)
		return status;
	}
	else
	    return UNKNOWN;
    }
}

Boolean ServerDispatcher::createObject (Buffer* work[], int)
{
    Uid dispatcherType;
    DispatcherList* ptr = headOfCreateList;
    Boolean found = FALSE;

    dispatcherType.unpack(*work[0]);

    while ((!found) && (ptr != (DispatcherList*) 0))
    {
	if (ptr->_dispatcher->dispatcherName() == dispatcherType)
	    found = TRUE;
	else
	    ptr = ptr->_next;
    }

    if (!found)
	return FALSE;

    Dispatcher* tmp = ptr->_dispatcher->create();

    if (tmp)
    {
	ptr = new DispatcherList(tmp);
	ptr->_next = headOfObjectList;
	headOfObjectList = ptr;

	return TRUE;
    }

    return FALSE;
}
					
Dispatcher::DispatcherStatus ServerDispatcher::callDispatcher (Buffer* work[], int nRec)
{
    Dispatcher::DispatcherStatus status = Dispatcher::DONE;
    DispatchOpcode opcode;
    Boolean errorCode = FALSE;
    Uid dispatchID;
    Buffer errorBuff, resultBuff;
    Buffer* result[MAXBUFFERS];
    int resultSize = 1;

    for (int i = 0; i < MAXBUFFERS; i++)
        result[i] = (Buffer*) 0;

    if (nRec < 1)
	errorCode = TRUE;
    else
    {
	int dummyCode = 0;
	
	work[0]->unpack(dummyCode);
	opcode = (DispatchOpcode) dummyCode;

	switch (opcode)
	{
	case D_CREATE:
	    {
		if (createObject(work, nRec) == FALSE)
		    errorCode = TRUE;
		else
		    dispatchID = headOfObjectList->_dispatcher->dispatcherName();
	    }
	    break;
	case D_DELETE:
	    errorCode = TRUE;
	    break;
	case D_USERCODE:
	    dispatchID.unpack(*work[0]);
	    break;
	case D_NAME:
	    name.pack(resultBuff);
	    break;
	}

	cout << "errorCode now " << errorCode << endl;

	cout << "nRec " << nRec << endl;
	
	if ((errorCode == FALSE) && (nRec -1 > 0))
	{
	    DispatcherList* ptr = headOfObjectList;
	    Boolean found = FALSE;

	    while ((!found) && (!ptr))
	    {
		cout << "comparing " << ptr->_dispatcher->dispatcherName() << " and " << dispatchID << endl;
		
		if (ptr->_dispatcher->dispatcherName() == dispatchID)
		    found = TRUE;
		else
		    ptr = ptr->_next;
	    }

	    if (!found)
		errorCode = TRUE;
	    else		// if any more work buffers then pass them to the actual object (dispatcher).
		status = ((ptr->_dispatcher->dispatch(&work[1], nRec-1, &result[1], resultSize) == Dispatcher::DONE) ? DONE : UNKNOWN);
	}
	else
	{
	    dispatchID.pack(resultBuff);
	    resultSize++;
	    result[1] = &resultBuff;
	}
    }

    // now add error buffer to start of list.

    errorBuff.pack(errorCode);
    result[0] = &errorBuff;

    _rpcHandle.SendResult(result, resultSize);
    
    // delete result buffer

    for (i = 0; i < resultSize-1; i++)
    {
        if (result[i+1])
	   delete result[i+1];
    }

    return status;
}

Dispatcher::DispatcherStatus ServerDispatcher::dispatch (Buffer*[], int, Buffer*[], int&)
{
    return Dispatcher::NOTDONE;
}
