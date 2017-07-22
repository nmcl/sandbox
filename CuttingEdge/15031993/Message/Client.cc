#ifndef CLIENT_H_
#  include "Client.h"
#endif

#ifndef SEQUENCENUMBER_H_
#  include "SequenceNumber.h"
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#include <sys/types.h>
#include <netinet/in.h>

Client::Client ()
{
    set = FALSE;
}

Client::~Client () {}
    
Boolean Client::Initiate (GroupList& objects, int successes)
{
    Boolean result = FALSE;
 
    if (objects.getServerStatus() != UNINITIATED)
	return FALSE;
    
    if (initiate(objects.getServerList(), objects.getGroup(), &successes) == SUCCEEDED)
    {
	objects.setServerStatus(OPERATIONAL);
	result = TRUE;
    }
    else
	error_stream << WARNING << " ClientRajdoot_rpc::Initiate() : Failed\n";
    
    return result;
}

Boolean Client::Terminate (GroupList& objects)
{
    Boolean result = FALSE;
    
    if ((objects.getServerStatus() == OPERATIONAL) || (objects.getServerStatus() == INOPERATIONAL))
    {
	objects.setServerStatus(DEAD);
	terminate(objects.getGroup(), (Boolean) FALSE, objects.getServerList());
	result = TRUE;
    }
    else if (objects.getServerStatus() == UNINITIATED)
	error_stream << WARNING << " ClientRajdoot_rpc::Terminate() : Call on inactive servers.\n";
    else
	error_stream << WARNING << " ClientRajdoot_rpc::Terminate() : Call on (soft) dead servers.\n";

    return result;
}

Boolean Client::SendMessage (GroupID* cl_addr, Buffer toSend, SequenceNumber sn)
{
    mc_msghdr req_buf;
    long indicate_opcode = htonl(USER_RPC), opcode = sn.getOpcode();
    MID red_gmid;
    Boolean sentOK = TRUE;
    
    if (!cl_addr)
	return FALSE;
    
    ClientData *ptr = cl_addr->getList();

    char* packed_ags = new char[(sizeof(long) + (2*sizeof(MID)) -8)];

    ::memset(packed_ags, '\0', sizeof(packed_ags));
    
    req_buf.mc_msg[0].iov_base = (char*) sn.getSequenceNumber();
    req_buf.mc_msg[0].iov_len = sizeof(RPC_SN);
    req_buf.mc_msg[1].iov_base = (char*) &indicate_opcode;
    req_buf.mc_msg[1].iov_len = sizeof(indicate_opcode);
    req_buf.mc_msg[2].iov_base = (char*) &red_gmid;
    req_buf.mc_msg[2].iov_len = sizeof(MID);
    req_buf.mc_msg[3].iov_base = (char*) &opcode;
    req_buf.mc_msg[3].iov_len = sizeof(opcode);
    req_buf.mc_msg[4].iov_base = (char*) packed_ags;
    req_buf.mc_msg[4].iov_len = sizeof(packed_ags);
    req_buf.mc_msg[5].iov_base = (char*) toSend.buffer();
    req_buf.mc_msg[5].iov_len = toSend.length();
    req_buf.mc_msg[6].iov_base = 0;
    req_buf.mc_msg[6].iov_len = 0;
    req_buf.msg_len = 7;

    while (ptr && sentOK)
    {
	MID *destn = ptr->myClient->getClientAddr();

	if (multicast_long_send(destn, &req_buf) == MC_OK)
	{
	    ptr = ptr->next;
	    sentOK = TRUE;
	}
	else
	    sentOK = FALSE;
    }
    
    return sentOK;
}

Boolean Client::SendRequest (GroupList& objects, Buffer toSend, SequenceNumber sn)
{
    mc_msghdr req_buf;
    long indicate_opcode = htonl(USER_RPC), opcode = sn.getOpcode();
    MID red_gmid;
    char* packed_ags = new char[(sizeof(long) + (2*sizeof(MID)) -8)];

    ::memset(packed_ags, '\0', sizeof(packed_ags));
    
    req_buf.mc_msg[0].iov_base = (char*) sn.getSequenceNumber();
    req_buf.mc_msg[0].iov_len = sizeof(RPC_SN);
    req_buf.mc_msg[1].iov_base = (char*) &indicate_opcode;
    req_buf.mc_msg[1].iov_len = sizeof(indicate_opcode);
    req_buf.mc_msg[2].iov_base = (char*) &red_gmid;
    req_buf.mc_msg[2].iov_len = sizeof(MID);
    req_buf.mc_msg[3].iov_base = (char*) &opcode;
    req_buf.mc_msg[3].iov_len = sizeof(opcode);
    req_buf.mc_msg[4].iov_base = (char*) packed_ags;
    req_buf.mc_msg[4].iov_len = sizeof(packed_ags);
    req_buf.mc_msg[5].iov_base = (char*) toSend.buffer();
    req_buf.mc_msg[5].iov_len = toSend.length();
    req_buf.mc_msg[6].iov_base = 0;
    req_buf.mc_msg[6].iov_len = 0;
    req_buf.msg_len = 7;
    
    if (multicast_send_message(objects.getServerList(), &req_buf) == MC_OK)
	return TRUE;
    else
	return FALSE;
}

Boolean Client::ReceiveMessage (Buffer& result, long timeout, SequenceNumber& sn)
{
    mc_msghdr result_buf;
    mc_sg_filter ack;
    long opcode = ZEROCODE;
    
    result_buf.mc_msg[0].iov_base = 0;
    result_buf.mc_msg[0].iov_len = sizeof(RPC_SN);
    result_buf.mc_msg[1].iov_base = 0;
    result_buf.mc_msg[1].iov_len = sizeof(opcode);
    result_buf.mc_msg[2].iov_base = 0;
    result_buf.mc_msg[2].iov_len = 0;
    result_buf.mc_msg[3].iov_base = 0;
    result_buf.mc_msg[3].iov_len = 0;
    result_buf.mc_msg[4].iov_base = 0;
    result_buf.mc_msg[4].iov_len = 0;
    result_buf.mc_msg[5].iov_base = 0;
    result_buf.mc_msg[5].iov_len = 0;
    result_buf.mc_msg[6].iov_base = 0;
    result_buf.mc_msg[6].iov_len = 0;
    result_buf.msg_len = 7;
    ack.next_fs = 0;
    ack.mc_msg = &result_buf;
    
    setMyAddr();
    
    if (multicast_long_receive(&client, &ack, 0, timeout) > 1)
    {
	result.setBuffer(result_buf.mc_msg[5].iov_len, result_buf.mc_msg[5].iov_base);
	opcode = *(long*)result_buf.mc_msg[1].iov_base;
	sn.setOpcode(opcode);
	sn = *(RPC_SN*)result_buf.mc_msg[0].iov_base;
	
	return TRUE;
    }
    else
	return FALSE;
}
    
Boolean Client::ReceiveReply (Buffer& result, long timeout, SequenceNumber& sn)
{
    mc_msghdr result_buf;
    mc_sg_filter ack;
    long opcode = ZEROCODE;

    result_buf.mc_msg[0].iov_base = 0;
    result_buf.mc_msg[0].iov_len = sizeof(RPC_SN);
    result_buf.mc_msg[1].iov_base = 0;
    result_buf.mc_msg[1].iov_len = sizeof(opcode);
    result_buf.mc_msg[2].iov_base = 0;
    result_buf.mc_msg[2].iov_len = 0;
    result_buf.mc_msg[3].iov_base = 0;
    result_buf.mc_msg[3].iov_len = 0;
    result_buf.msg_len = 4;
    ack.next_fs = 0;
    ack.mc_msg = &result_buf;

    setMyAddr();
    
    if (multicast_long_receive(&client, &ack, 0, timeout) > 1)
    {
	result.setBuffer(result_buf.mc_msg[2].iov_len, result_buf.mc_msg[2].iov_base);
	opcode = *(long*)result_buf.mc_msg[1].iov_base;
	sn.setOpcode(opcode);
	sn = *(RPC_SN*)result_buf.mc_msg[0].iov_base;
	
	return TRUE;
    }
    else
	return FALSE;
}

void Client::setMyAddr ()
{
    if (!set)
    {
	set = TRUE;
	multicast_get_mid(&client);
    }
}

Boolean Client::pack (Buffer& buff)
{
    Boolean result;
    
    setMyAddr();
    
    result = buff.pack(client.type) && buff.pack(client.hi_part) && buff.pack(client.lo_part);
    
    return result;
}

Boolean Client::unpack (Buffer buff)
{
    Boolean result;
    long a = 0, b = 0, c = 0;

    result = buff.unpack(a) && buff.unpack(b) && buff.unpack(c);
    client.type = a;
    client.hi_part = b;
    client.lo_part = c;
    set = TRUE;

    return result;
}

MID* Client::getClientAddr ()
{
    setMyAddr();
    
    return &client;
}
