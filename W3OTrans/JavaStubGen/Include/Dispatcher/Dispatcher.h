/*
 * Copyright 1990, 1991, 1992,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: Dispatcher.h,v 1.2 1993/10/22 11:50:42 nsmw Exp $
 */


#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef MAXBUFFERS
#define MAXBUFFERS 10
#endif

enum DispatchOpcode { D_CREATE, D_DELETE, D_USERCODE, D_NAME };


class Dispatcher
{
public:
    enum DispatcherStatus { DONE, NOTDONE, UNKNOWN, TERMINATE };

    virtual ~Dispatcher () {};

    virtual DispatcherStatus dispatch (Buffer* work[], int nWork,
				       Buffer* result[], int& nResult) = 0;

    virtual Dispatcher* create () { return 0; };

    const Uid& dispatcherName () const { return name; };
    void setDispatcherName (const Uid& u) { name = u; }
    
protected:
    Dispatcher () {};

    Uid name;
};


#endif
