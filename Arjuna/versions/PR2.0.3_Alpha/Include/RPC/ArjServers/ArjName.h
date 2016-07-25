/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


#ifndef _ARJNAME_H_

#ifndef STUB
#  include <RPC/ArjServers/ArjName_stub.h>
#else

#define _ARJNAME_H_

/*
 * $Id: ArjName.h,v 1.14 1993/10/20 16:44:22 n048z Exp $
 *
 * Arjuna Name Implementation
 *
 */

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
