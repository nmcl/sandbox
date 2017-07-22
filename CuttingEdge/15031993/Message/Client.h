#ifndef CLIENT_H_
#define CLIENT_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

#ifndef RPC_H_
#  include "rpc.h"
#endif

#ifndef SEQUENCENUMBER_H_
#  include "SequenceNumber.h"
#endif

#ifndef GROUPLIST_H_
#  include "GroupList.h"
#endif

#ifndef GROUPVIEW_H_
#  include "GroupID.h"
#endif

class Client
{
public:
    Client ();
    ~Client ();

    Boolean Initiate (GroupList&, int);
    Boolean Terminate (GroupList&);

    Boolean SendRequest (GroupList&, Buffer, SequenceNumber);
    Boolean ReceiveReply (Buffer&, long, SequenceNumber&);

    Boolean SendMessage (GroupID*, Buffer, SequenceNumber);    
    Boolean ReceiveMessage (Buffer&, long, SequenceNumber&);

    Boolean pack (Buffer&);
    Boolean unpack (Buffer);

private:
    MID client;
    Boolean set;

    void setMyAddr ();
    
public:
    MID* getClientAddr ();
};

#endif


