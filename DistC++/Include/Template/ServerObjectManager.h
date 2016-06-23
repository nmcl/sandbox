/*
 * Copyright (C) 1993-1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerObjectManager.h,v 1.1 1997/09/25 15:28:00 nmcl Exp $
 */

#ifndef SERVEROBJECTMANAGER_H_
#define SERVEROBJECTMANAGER_H_

#include <StubGen/RpcBuffer.h>

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

template <class T>
class ServerObjectManager
{
public:
    ServerObjectManager (T* obj) { _theRealObject = obj; _constructOpcode_ = _constructRefCount_ = 0; };
    virtual ~ServerObjectManager () {};

    Uid get_uid () const { return _uid; };

    T*  _theRealObject;
    Uid _uid;
    RpcBuffer _constructArgs_;
    Int32 _constructOpcode_;
    Int32 _constructRefCount_;
    RpcBuffer _constructResult_;
};

#endif
