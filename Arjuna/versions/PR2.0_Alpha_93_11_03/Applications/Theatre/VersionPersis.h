/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: VersionPersis.h,v 1.1 1993/11/03 12:24:41 nmcl Exp $
 */

#ifndef VERSIONPERSIS_H_
#define VERSIONPERSIS_H_

#include <Common/Uid.h>
#include <Arjuna/ObjState.h>
#include <Arjuna/LockMan.h>

// @NoRemote, @NoMarshall
class VersionPersis :public LockManager
{
public:
    static VersionPersis* Create(int& res);
    static VersionPersis* Create(int& res, const Uid&);
    static void        Destroy();

    void increase(int& res);
    void get(int& res, int& version);

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;

private:
    VersionPersis(int& res);
    VersionPersis(int& res, const Uid& U);
    ~VersionPersis();

    int _version;

    static VersionPersis *listHead;
    VersionPersis        *nextElem;
};

#endif
