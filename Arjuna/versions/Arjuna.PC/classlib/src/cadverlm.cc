/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverLM.cc,v 1.2 1993/11/16 09:55:30 ngdp Exp $
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
#  include "CadverLM.h"
#endif

static const char RCSid[] = "$Id: CadaverLM.cc,v 1.2 1993/11/16 09:55:30 ngdp Exp $";

/*
 * Public non-virtual functions 
 */


CadaverLockManager::CadaverLockManager ( const Uid& objUid, const TypeName tName )
                                       : LockManager(objUid),
					 objectTypeName(tName)

{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "CadaverLockManager::CadaverLockManager(" << objUid << ")" << endl;
#endif
}


CadaverLockManager::~CadaverLockManager ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "CadaverLockManager::~CadaverLockManager()" << endl;
#endif
}

/*
 * Publically inherited functions
 */


Boolean CadaverLockManager::save_state ( ObjectState&, ObjectType )
{
    return FALSE;
}

Boolean CadaverLockManager::restore_state ( ObjectState&, ObjectType )
{
    return FALSE;
}


const TypeName CadaverLockManager::type () const
{
    return objectTypeName;
    
}
