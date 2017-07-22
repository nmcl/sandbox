#ifndef GROUPID_H_
#define GROUPID_H_

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
 
class Client;

class ClientData
{
public:
    ClientData ();
    ~ClientData ();
    
    Client* myClient;
    ClientData* next;
};


class GroupID
{
public:
    GroupID ();
    ~GroupID ();

    void addMember (Client*);
    void deleteMember (Client*);

    int getNumberOfMembers () const;
    
    Boolean pack (Buffer&) const;
    Boolean unpack (Buffer);
    
private:
    void deleteList ();
    
    ClientData* head;
    int numberOfMembers;

public:
    ClientData* getList () const;
};


#endif
    


