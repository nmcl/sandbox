/*
 * Copyright 1990, 1991, 1992,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: ServerDispatcher.h,v 1.1 1994/09/09 13:11:43 nmcl Exp nmcl $
 */

#ifndef SERVERDISPATCHER_H_
#define SERVERDISPATCHER_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef MULTIDISPATCHER_H_
#  include <Dispatcher/MultiDispatcher.h>
#endif

#ifndef SIMPLERPCSERVER_H_
#  include <RPC/SimpleRpc/SimpleRpcServer.h>
#endif

class PortAddress;
class SimpleRpcServer;


class DispatcherList
{
public:
    DispatcherList (Dispatcher*);
    ~DispatcherList ();

    Dispatcher* _dispatcher;
    DispatcherList* _next;
};


class ServerDispatcher : public MultiDispatcher
{
public:
    ServerDispatcher ();
    virtual ~ServerDispatcher ();
    
    virtual int initialise (int, char**);

    virtual Dispatcher::DispatcherStatus begin ();
    virtual Dispatcher::DispatcherStatus end ();

    virtual Dispatcher::DispatcherStatus addDispatcher (Dispatcher*);
    
protected:
    virtual Dispatcher::DispatcherStatus dispatch (Buffer*[], int, Buffer*[], int&);
    Dispatcher::DispatcherStatus callDispatcher (Buffer* work[], int nWork);

private:
    Boolean createObject (Buffer* work[], int nWork);
    
    SimpleRpcServer _rpcHandle;
    DispatcherList* headOfCreateList;
    DispatcherList* headOfObjectList;
};

#endif
