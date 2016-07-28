/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

#ifndef COMMON_ORBINTERFACE_H_
#  include <Common/ORBInterface.h>
#endif

CORBA::ORB_ptr ORBInterface::_orb = CORBA_ORB_(_nil)();
CORBA::BOA_ptr ORBInterface::_boa = CORBA_BOA_(_nil)();

ORBInterface::ORBInterface ()
{
}

ORBInterface::~ORBInterface ()
{
}

CORBA::ORB_ptr ORBInterface::initORB (int& argc, char** argv, ORBid id, CORBA_(Environment& env))
{
    if (CORBA_(is_nil)(_orb))
	_orb = CORBA::ORB_init(argc, argv, id, env);

    CORBA_ORB_(_duplicate)(_orb);

    return _orb;
}

CORBA::BOA_ptr ORBInterface::initBOA (int& argc, char** argv, OAid id, CORBA_(Environment& env))
{
    if (CORBA_(is_nil)(_orb))
	return _boa;  // must be nil by definition

    if (CORBA_(is_nil)(_boa))
	_boa = _orb->BOA_init(argc, argv, id, env);

    CORBA_BOA_(_duplicate)(_boa);

    return _boa;
}

CORBA::ORB_ptr ORBInterface::orb ()
{
    return CORBA_(_duplicate)(_orb);
}

CORBA::BOA_ptr ORBInterface::boa ()
{
    return CORBA_(_duplicate)(_boa);
}
