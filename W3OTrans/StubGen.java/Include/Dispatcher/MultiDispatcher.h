/*
 * Copyright 1990, 1991, 1992,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: MultiDispatcher.h,v 1.2 1993/10/22 11:50:42 nsmw Exp $
 */


#ifndef MULTIDISPATCHER_H_
#define MULTIDISPATCHER_H_

#ifndef DISPATCHER_H_
#  include <Dispatcher/Dispatcher.h>
#endif


class MultiDispatcher : public Dispatcher
{
public:
    virtual ~MultiDispatcher () {};

    virtual DispatcherStatus addDispatcher (Dispatcher*) = 0;

protected:
    MultiDispatcher () {};
};

#endif
