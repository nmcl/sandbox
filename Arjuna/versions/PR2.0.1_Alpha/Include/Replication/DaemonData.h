/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DaemonData.h,v
 */

#ifndef DAEMONDATA_H_
#define DAEMONDATA_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

class Members;


// @NoRemote, @NoMarshall
class DaemonData
{
public:
    DaemonData ();
    ~DaemonData ();

    Members* memb;
    char* hostName;
    Boolean available;
};

#endif
