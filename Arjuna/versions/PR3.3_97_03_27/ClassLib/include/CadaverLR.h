/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverLR.h,v 1.4 1993/12/14 11:28:50 nmcl Exp $
 */

#ifndef CADAVERLR_H_
#define CADAVERLR_H_

/*
 *
 * Record to handle dangling locks that may be created if the object
 * dies prior to committing
 *
 */

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef LOCKR_H_
#  include "LockR.h"
#endif

class LockManager;
class Lockstore;
class Uid;
class ostream;

class CadaverLockRecord : public LockRecord
{
    friend class CadaverLSetUp;

public:
    /* Constructors and destructor */

    CadaverLockRecord (const key_t, LockManager *);
    virtual ~CadaverLockRecord ();

    /* inherited functions */

    virtual Boolean propagateOnAbort () const;

    virtual Boolean nestedAbort ();
    virtual Boolean nestedCommit ();
    virtual Boolean topLevelAbort ();
    virtual Boolean topLevelCommit ();

    virtual const TypeName type () const;
    virtual ostream& print (ostream &) const;

protected:
    CadaverLockRecord ();

private:
    virtual Boolean shouldReplace (const AbstractRecord *) const;

    LockStore *cadaverLockStore;
    const TypeName objectTypeName;
    
};

class CadaverLSetUp
{
public:
    CadaverLSetUp ();
    ~CadaverLSetUp ();

    static AbstractRecord* create ();
    static void remove (AbstractRecord*&);

private:
    static Boolean setUp;
};

static CadaverLSetUp CadLSetUp;


#endif























