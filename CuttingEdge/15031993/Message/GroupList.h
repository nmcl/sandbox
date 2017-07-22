#ifndef GROUPLIST_H_
#define GROUPLIST_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef CLSTATUS_H_
#  include <RPC/ClStatus.h>
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

#ifndef RPC_H_
#  include "rpc.h"
#endif
 

class GroupList
{
public:
    GroupList (int);
    ~GroupList ();
    
    Boolean addName (const char*, const char* = 0);
    Boolean modifyName (const char*, int, const char* = 0);
    
    Boolean pack (Buffer&) const;
    Boolean unpack (Buffer);
    
private:
    Boolean addName (const char*, const char*, int);

    GROUPS* newgroup;
    OBJECT_DESC* objects;
    int numberOfServers, total;
    ServerStatus status;

public:
    OBJECT_DESC* getServerList () const;
    GROUPS* getGroup ();

    ServerStatus getServerStatus () const;
    void setServerStatus (ServerStatus);
};

#endif
