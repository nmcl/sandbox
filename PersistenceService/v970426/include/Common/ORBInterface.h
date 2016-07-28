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
#define COMMON_ORBINTERFACE_H_

#ifndef COMMON_NAMESPACE_H_
#  include <Common/nameSpace.h>
#endif

#ifndef COMMON_CORBANAMES_H_
#  include <Common/CorbaNames.h>
#endif

#include <corba.h>

class ORBInterface
{
public:
    static CORBA::ORB_ptr initORB (int&, char**, ORBid, CORBA_(Environment& env));
    static CORBA::BOA_ptr initBOA (int&, char**, OAid, CORBA_(Environment& env));

    static CORBA::ORB_ptr orb ();
    static CORBA::BOA_ptr boa ();
    
private:
    ORBInterface ();
    ~ORBInterface ();

    static CORBA::ORB_ptr _orb;
    static CORBA::BOA_ptr _boa;
};

#endif

