#ifndef SEQUENCENUMBER_H_
#  include "SequenceNumber.h"
#endif

SequenceNumber::SequenceNumber ()
{
    seqNo = 0;
}

SequenceNumber::SequenceNumber (RPC_SN sn)
{
    seqNo = new RPC_SN;
    
    *seqNo = sn;
}

SequenceNumber::~SequenceNumber ()
{
    if (seqNo)
	delete seqNo;
}

void SequenceNumber::selectSN ()
{
    getNewSequenceNumber();
}

RPC_SN* SequenceNumber::getSequenceNumber ()
{
    if (!seqNo)
	getNewSequenceNumber();
    
    return seqNo;
}

void SequenceNumber::getNewSequenceNumber ()
{
    if (!seqNo)
	seqNo = new RPC_SN;
    
    (void)getsn(seqNo);
}

void SequenceNumber::getContents (long& sec, long& msec) const
{
    sec = seqNo->sec;
    msec = seqNo->msec;
}

long SequenceNumber::getOpcode () { return opcode; }

void SequenceNumber::setOpcode (long newOpcode) { opcode = newOpcode; }

SequenceNumber& SequenceNumber::operator= (const RPC_SN& sn)
{
    if (!seqNo)
	seqNo = new RPC_SN;
    
    seqNo->sec = sn.sec;
    seqNo->msec = sn.msec;
    seqNo->retry = sn.retry;
    seqNo->t_out = sn.t_out;
    
    return *this;
}

Boolean SequenceNumber::operator== (const SequenceNumber& sn)
{
    long sec = 0, msec = 0;
    sn.getContents(sec, msec);
    return ((seqNo->sec == sec) && (seqNo->msec == msec));
}

    
