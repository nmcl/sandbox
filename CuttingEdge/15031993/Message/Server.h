#ifndef SERVER_H_
#define SERVER_H_

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

class Server
{
public:
    Server ();
    ~Server ();

    Boolean Initialise (int, char**);
    
    void GetWork (SequenceNumber&, Buffer&);
    void SendResult (long, Buffer&);
    
    Boolean pack(Buffer&) const;
    Boolean unpack (Buffer);
    
private:
    Boolean init;
    
    GROUPS* client_addr;
    DEADLINE deadlines;
    MID myaddress;
    
public:
    static char* ServerName;
};

#endif
