#ifndef SEQUENCENUMBER_H_
#define SEQUENCENUMBER_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

#ifndef RPC_H_
#  include "rpc.h"
#endif
 
class SequenceNumber
{
public:
    SequenceNumber ();
    ~SequenceNumber ();
    
    long getOpcode ();
    void setOpcode (long);

    void selectSN ();
    
    Boolean operator== (const SequenceNumber&);

private:
    void getNewSequenceNumber ();
    
    RPC_SN* seqNo;
    long opcode;

public:
    SequenceNumber (RPC_SN);

    RPC_SN* getSequenceNumber ();
    void getContents (long&, long&) const;

    SequenceNumber& operator= (const RPC_SN&);
};

#endif
