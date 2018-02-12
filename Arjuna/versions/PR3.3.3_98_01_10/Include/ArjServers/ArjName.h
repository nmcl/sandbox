/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArjName.h,v 1.6 1995/07/11 13:57:50 nmcl Exp $
 *
 */

#ifndef _ARJNAME_H_

#ifndef STUB
#  include <ArjServers/ArjName_stub.h>
#else

#define _ARJNAME_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef STATEMAN_H_
#  include <Arjuna/StateMan.h>
#endif

#ifndef COMMONT_H_
#  include <Arjuna/CommonT.h>
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
    ArjunaName (const char* const name);
    ~ArjunaName ();

    Boolean lookUp ();
    Boolean registerName ();
    Boolean registerName (const char * const, const StateManager&);

    void setObjectName (const char * const);
    void setObjectUid (const Uid&);
    void setServiceName (const char * const);
    void setHostName (const char * const);
    void setObjectStoreRoot (const char * const);

    Boolean valid () const;
    
    Uid   getObjectUid () const;

    const char* const getObjectName () const;
    const char* const getServiceName () const;
    const char* const getHostName () const;
    const char* const getObjStoreRoot () const;

    Boolean pack (Buffer&) const;
    Boolean unpack (Buffer&);

    /* Rpc marshalling */

    void marshall (RpcBuffer&) const;
    void unmarshall (RpcBuffer&);
    
    ostream& print ( ostream& strm ) const;

private:
    char* objectName;
    Uid   objectUid;
    char* serviceName;
    char* hostName;
    char* OSRoot;
    NSInterface* NShandle;
    Boolean _valid;
};

extern ostream& operator<< ( ostream& strm, const ArjunaName& an );

#endif
#endif
