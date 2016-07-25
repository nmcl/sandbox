/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverLM.cc,v 1.1 1993/11/03 12:29:46 nmcl Exp $
 */

/*
 *
 * Lock concurrency controller
 *
 * Lock-base concurrency control management system
 * Instances of this class are created by CadaverLockRecord class
 * instances for the sole purpose of lock cleanup due to a locked
 * object going out of scope prior to action termination. 
 * Serialisability prevents locks being released as scope is exited
 * thus they must be cleaned up later.
 *
 */

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef CADAVERLM_H_
#  include "CadaverLM.h"
#endif

static const char RCSid[] = "$Id: CadaverLM.cc,v 1.1 1993/11/03 12:29:46 nmcl Exp $";

/*
 * Public non-virtual functions 
 */

CadaverLockManager::CadaverLockManager ( const Uid& objUid, const TypeName tName )
                                       : LockManager(objUid),
					 objectTypeName(tName)

{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "CadaverLockManager::CadaverLockManager(" << objUid << ")\n" << flush;
#endif
}

CadaverLockManager::~CadaverLockManager ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "CadaverLockManager::~CadaverLockManager()\n" << flush;
#endif
}

/*
 * Publically inherited functions
 */

Boolean CadaverLockManager::restore_state ( ObjectState&, ObjectType )
{
    return FALSE;
}


Boolean CadaverLockManager::save_state ( ObjectState&, ObjectType )
{
    return FALSE;
}

const TypeName CadaverLockManager::type () const
{
    return objectTypeName;
    
}
