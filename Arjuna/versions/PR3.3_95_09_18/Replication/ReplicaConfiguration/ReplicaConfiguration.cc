/*
 * Copyright (C) 1993, 1994, 1995, 1996
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ReplicaConfiguration.cc,v 1.4 1996/07/16 16:55:50 nmcl Exp $
 */

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef REPLICACONFIGURATION_H_
#  include <Replication/ReplicaConfiguration.h>
#endif


ReplicaConfiguration::ReplicaConfiguration (const char* fileName, Boolean& res)
					   : _hosts(0),
					     _number(0)
{
    res = FALSE;
    
    if (fileName)
    {
	ifstream input(fileName);

	if (!input)
	{
	    error_stream << WARNING << "ReplicaConfiguration file " << fileName << " not found." << endl;
	    return;
	}
	
	if (input.peek() != EOF)
	{
	    input >> _number;

	    if (_number > 0)
	    {
		int i;
		
		_hosts = ::new char* [_number];

		for (i = 0; (i < _number) && (input.peek() != EOF); i++)
		{
		    _hosts[i] = ::new char[MAXHOSTNAMELEN+1];
		    ::memset(_hosts[i], '\0', MAXHOSTNAMELEN+1);

		    input >> _hosts[i];
		}
		
		if (i == _number)
		    res = TRUE;
	    }

	    if (!res)
		_number = 0;
	}
    }
}

ReplicaConfiguration::~ReplicaConfiguration ()
{
    if (_number > 0)
    {
	for (int i = 0; i < _number; i++)
	    ::delete [] _hosts[i];

	::delete _hosts;
    }
}

char** ReplicaConfiguration::hosts () const
{
    return _hosts;
}

int ReplicaConfiguration::number () const
{
    return _number;
}
