/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverLR.h,v 1.1 1993/11/03 12:28:48 nmcl Exp $
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
public:
    /* Constructors and destructor */

    CadaverLockRecord (const key_t storeKey, LockManager *lm);
    virtual ~CadaverLockRecord ();

    /* inherited functions */

    virtual Boolean should_replace (const AbstractRecord *ar) const;

    virtual void nested_abort ();
    virtual void nested_commit ();
    virtual void top_level_abort ();
    virtual void top_level_commit ();

    virtual const TypeName type () const;
    virtual ostream& print (ostream & strm) const;
private:
    LockStore *cadaverLockStore;
    const TypeName objectTypeName;
    
};

#endif























