/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ClRajAct_.cc,v 1.4 1995/06/26 10:33:57 ngdp Exp $
 */

static const char RCSid[] = "$Id: ClRajAct_.cc,v 1.4 1995/06/26 10:33:57 ngdp Exp $";

#ifdef  DEBUG
#  ifndef DEBUG_H_
#    include <Common/Debug.h>
#  endif
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef RAJDOOT_CLRAJACT_H_
#  include <RPC/Rajdoot/ClRajAct.h>
#endif

ClientRajdootAction_rpc::ClientRajdootAction_rpc(ArjunaName* ArjName) 
                                                : ClientRajdoot_rpc(ArjName),
                                                  atomicEnabled(TRUE),
						  isZombie(FALSE),
						  objUid()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ClientRajdootAction_rpc::ClientRajdootAction_rpc(ArjName*)\n" << flush;
    debug_stream.unlock();
    
#endif
}
