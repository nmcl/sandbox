/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


#ifndef _ARJNAME_H_
#define _ARJNAME_H_

/*
 * $Id: ArjName.h,v 1.9 1993/03/18 14:29:52 nmcl Exp $
 *
 * Arjuna Name Implementation
 *
 */

#ifdef STUB
#  pragma @NoRename
#endif

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

// @NoRemote, @UserMarshall
class Buffer;

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

    ostream& print ( ostream& strm ) const;

private:
    char* ObjName;
    Uid   ObjUid;
    char* ServiceName;
    char* HostName;
    class NSInterface* NS;
};

extern ostream& operator<< ( ostream& strm, const ArjunaName& an );

extern Buffer& operator<< (Buffer&, const ArjunaName&);
extern Buffer& operator>> (Buffer&, ArjunaName&);
extern Buffer& operator<< (Buffer&, ArjunaName*);
extern Buffer& operator>> (Buffer&, ArjunaName*&);

#endif
