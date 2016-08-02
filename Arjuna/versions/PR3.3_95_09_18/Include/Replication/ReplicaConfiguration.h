/*
 * Copyright (C) 1993, 1994, 1995, 1996
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ReplicaConfiguration.h,v 1.1 1996/07/05 12:58:15 nmcl Exp $
 */

#ifndef REPLICACONFIGURATION_H_
#define REPLICACONFIGURATION_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

class ReplicaConfiguration
{
public:
    ReplicaConfiguration (const char* fileName, Boolean& res);
    ~ReplicaConfiguration ();

    char** hosts  () const;
    int    number () const;

private:
    char** _hosts;
    int    _number;
};

#endif
