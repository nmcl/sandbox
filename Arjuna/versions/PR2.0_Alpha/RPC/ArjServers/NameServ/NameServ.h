/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


#ifndef NAMESERV_H_
#define NAMESERV_H_

/*
 * $Id: NameServ.h,v 1.6 1993/03/18 15:24:33 nmcl Exp $
 *
 * Primitive Name Server 
 *
 */

#ifndef UID_H_
#   include <Common/Uid.h>
#endif

#ifndef BUFFER_H_
#   include <Common/Buffer.h>
#endif

#ifndef BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

// @NoRemote, @NoMarshall
struct DBM;

// @Remote, @NoMarshall
class NameServer
{
	DBM *db;
	Boolean scanning;
public:
	NameServer(Boolean &done);
	virtual ~NameServer();

	void StoreName(Boolean&, Buffer, Buffer);
	void FetchName(Boolean&, Buffer, Buffer&);
	void RemoveName(Boolean&, Buffer);
	void Restart(Boolean&);
	void GetNextName(Boolean&, Buffer&, Boolean&);
};

#endif NAMESERV_H_
