/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: ArjName.h,v 1.1 1993/11/03 14:27:21 nmcl Exp $
 *
 * Arjuna Name Implementation
 *
 */

#ifndef _ARJNAME_H_

#ifndef STUB
#  include <ArjServers/ArjName_stub.h>
#else

#define _ARJNAME_H_

#ifndef UID_H_
#include <Common/Uid.h>
#endif

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#ifndef STATEMAN_H_
#include <Arjuna/StateMan.h>
#endif

#ifndef COMMONT_H_
#include <Arjuna/CommonT.h>
#endif


// @NoRemote, @NoMarshall
class ostream;

// @NoRemote, @NoMarshall
class NSInterface;


// @NoRemote, @UserMarshall
class ArjunaName
{
public:
    /* Constructors & destructor */

    ArjunaName ();
    ArjunaName (const char* Name);
    ~ArjunaName ();

    void LookUp();
    void Register();
    void Register(const char*, StateManager*);

    void SetObjName(const char*);
    void SetObjUid(const Uid&);
    void SetServiceName(const char*);
    void SetHostName(const char*);

    char* GetObjName();
    Uid   GetObjUid();
    char* GetServiceName();
    char* GetHostName();

    char* GetRefObjName();
    Uid   GetRefObjUid();
    char* GetRefServiceName();
    char* GetRefHostName();

    Boolean pack (Buffer&) const;
    Boolean unpack (Buffer&);

    /* Rpc marshalling */

    void marshall (RpcBuffer&) const;
    void unmarshall (RpcBuffer&);
    
    ostream& print ( ostream& strm ) const;

private:
    char* ObjName;
    Uid   ObjUid;
    char* ServiceName;
    char* HostName;
    NSInterface* NS;
};

extern ostream& operator<< ( ostream& strm, const ArjunaName& an );

#endif
#endif
