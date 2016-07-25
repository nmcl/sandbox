/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverLM.h,v 1.1 1993/11/03 12:28:46 nmcl Exp $
 */

#ifndef CADAVERLM_H_
#define CADAVERLM_H_

/*
 *
 * Lock-base concurrency control management system
 * Instances of this class are created by CadaverLockRecord class
 * instances for the sole purpose of lock cleanup due to a locked
 * object going out of scope prior to action termination. 
 * Serialisability prevents locks being released as scope is exited
 * thus they must be cleaned up later.
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

class Uid;

class CadaverLockManager : public LockManager
{
public:
    /* Constructors and destructor */

    CadaverLockManager (const Uid& objUid, const TypeName tName);
    ~CadaverLockManager ();

    /* virtual functions inherited from LockManager */ 

    virtual Boolean restore_state (ObjectState& os, ObjectType ot);
    virtual Boolean save_state (ObjectState& os, ObjectType ot);
    virtual const TypeName type () const;
private:

    const TypeName objectTypeName;
    
};

#endif
