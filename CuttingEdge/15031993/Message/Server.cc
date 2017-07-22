#ifndef SERVER_H_
#  include "Server.h"
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

Server::Server ()
{
    init = FALSE;
    client_addr = 0;
}

Server::~Server ()
{
    if (client_addr)
	delete client_addr;
}

Boolean Server::Initialise (int argc, char** argv)
{
    int res;
    
    ServerName = strdup(argv[0]);
    
    if (res = (init_server(argc, argv)) == DIE)
	exit(91);
    
    init = (res == SUCCEEDED);
    
    return init;
}

void Server::GetWork (SequenceNumber& sn, Buffer& call)
{
    long opcode;
    int res = SUCCEEDED;
    
    if (!init)
	return;
    
    RPC rpc_work;
    
    rpc_work.rpc_code = 0;
    rpc_work.rpc_buff1 = 0;
    rpc_work.rpc_length1 = 0;
    rpc_work.rpc_buff2 = 0;
    rpc_work.rpc_length2 = 0;
    
    if (client_addr == NULL)
	client_addr = new GROUPS;
    
    do
	res = getwork(client_addr, &rpc_work, &deadlines, 0);
    while (res != SUCCEEDED);
    
    opcode = rpc_work.rpc_code;
    sn.setOpcode(opcode);
    
    if ((opcode == 0) && (rpc_work.rpc_buff1 == NULL) &&
	(rpc_work.rpc_length1 == 0))
    {
	error_stream << FATAL << "No buffer 1 in rpc structure.\n";
	  ArjunaFatal();
    }
    else
	call.setBuffer((size_t) rpc_work.rpc_length1, rpc_work.rpc_buff1);

    if (rpc_work.rpc_buff2 != 0)
	free(rpc_work.rpc_buff2);
}

void Server::SendResult (long errcode, Buffer& result)
{
    if (!init)
	return;

     RPC rpc_result;

     rpc_result.rpc_code    = errcode;
     rpc_result.rpc_buff1   = result.buffer();
     rpc_result.rpc_length1 = result.length();
     rpc_result.rpc_buff2   = NULL;
     rpc_result.rpc_length2 = 0;

     if (sendresult(client_addr, &rpc_result) != SUCCEEDED)
     {
          error_stream << FATAL << "Unable to send result.\n";
	  ArjunaFatal();
     }
}
    
    
