/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ClRajAct_.cc,v 1.3 1993/03/18 15:31:42 nmcl Exp $
 */

static const char RCSid[] = "$Id: ClRajAct_.cc,v 1.3 1993/03/18 15:31:42 nmcl Exp $";

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
						  is_zombie(FALSE),
						  obj_uid()
{
#ifdef DEBUG
        debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
  	debug_stream << "ClientRajdootAction_rpc::ClientRajdootAction_rpc(ArjName*)\n" << flush;
#endif
}
