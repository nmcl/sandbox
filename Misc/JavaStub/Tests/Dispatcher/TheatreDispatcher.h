/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TheatreDispatcher.h,v 1.1 1994/07/01 10:53:49 ngdp Exp $
 */

#ifndef THEATREDISPATCHER_H_
#define THEATREDISPATCHER_H_

#include <Config/Configure.h>

#ifndef DISPATCHER_H_
#  include <Dispatcher/Dispatcher.h>
#endif

#ifndef THEATRE_H_
#  include "Theatre.h"
#endif


class TheatreDispatcher : public Dispatcher
{
public:
    TheatreDispatcher ();
    virtual ~TheatreDispatcher ();

    virtual Dispatcher::DispatcherStatus dispatch (Buffer* work[], int nWork,
						   Buffer* result[], int& nResult);

    virtual Dispatcher* create ();

private:
    Theatre theRealObject;
};


#endif
