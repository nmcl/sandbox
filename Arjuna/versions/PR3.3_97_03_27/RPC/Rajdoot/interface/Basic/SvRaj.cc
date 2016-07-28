/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: SvRaj.cc,v 1.8 1995/07/03 14:29:36 ngdp Exp $
 */

static const char RCSid[] = "$Id: SvRaj.cc,v 1.8 1995/07/03 14:29:36 ngdp Exp $";

/*
 * This class provides C++ interface to the server side of the
 * Rajdoot rpc protocol.
 *
 */

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifdef DEBUG
#  ifndef DEBUG_H_
#    include <Common/Debug.h>
#  endif
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef RAJDOOT_SVRAJ_H_
#  include <RPC/Rajdoot/SvRaj.h>
#endif

static char* ServerName = NULL;

ServerRajdoot_rpc::ServerRajdoot_rpc()
{
/*
 * #ifdef DEBUG
 *         debug_stream.lock();
 *         debug_stream << CONSTRUCTORS << FAC_BASIC_RPC << VIS_PUBLIC;
 *         debug_stream << "ServerRajdoot_rpc::ServerRajdoot_rpc()\n" << flush;
 *         debug_stream.unlock();
 * #endif
 */

    init        = FALSE;
    client_addr = NULL;
}

ServerRajdoot_rpc::~ServerRajdoot_rpc()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdoot_rpc::~ServerRajdoot_rpc()\n" << flush;
    debug_stream.unlock();
    
#endif

    if (client_addr != NULL)
        delete client_addr;
}

int ServerRajdoot_rpc::initialise(int argc, char *argv[])
{
/*
 * #ifdef DEBUG
 *      debug_stream.lock();
 *      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
 * 	debug_stream << "ServerRajdoot_rpc::initialise(int argc, char *argv[])\n" << flush;
 *      debug_stream.unlock();
 * #endif
 */

    int res;

    ServerName = new char[strlen(argv[0])+1];
    strcpy(ServerName, argv[0]);
    
    if ((res = (init_server (argc, argv)) == DIE))
        exit(91);

    init = (res == SUCCEEDED);

    return init;
}

void ServerRajdoot_rpc::doGetWork (Int32& opcode, Buffer& call)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdoot_rpc::doGetWork (Int32& opcode, Buffer& call)\n" << flush;
    debug_stream.unlock();
    
#endif

     int res = SUCCEEDED;
     
     if (! init)
         return;

     RPC rpc_work;

     rpc_work.rpc_code    = 0;
     rpc_work.rpc_buff1   = NULL;
     rpc_work.rpc_length1 = 0;
     rpc_work.rpc_buff2   = NULL;
     rpc_work.rpc_length2 = 0;

     if (client_addr == NULL)
         client_addr = new GROUPS;

     do
         res = getwork(client_addr, &rpc_work, &deadlines, NULL);
     while (res != SUCCEEDED);

     opcode = rpc_work.rpc_code;
     if ((opcode == 0) && (rpc_work.rpc_buff1 == NULL) &&
	 (rpc_work.rpc_length1 == 0))
     {
          error_stream << FATAL << "No buffer 1 in rpc structure.\n";
	  ArjunaFatal();
     }
     else
          call.setBuffer((size_t) rpc_work.rpc_length1, rpc_work.rpc_buff1);

     if (rpc_work.rpc_buff2 != NULL)
         free(rpc_work.rpc_buff2);
}

void ServerRajdoot_rpc::doGetWork (Int32& opcode, Buffer& call1, Buffer& call2)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdoot_rpc::doGetWork (Int32& opcode, Buffer& call1, Buffer& call2)\n" << flush;
    debug_stream.unlock();
    
#endif

     int res = SUCCEEDED;
     
     if (! init)
         return;

     RPC rpc_work;

     rpc_work.rpc_code    = 0;
     rpc_work.rpc_buff1   = NULL;
     rpc_work.rpc_length1 = 0;
     rpc_work.rpc_buff2   = NULL;
     rpc_work.rpc_length2 = 0;

     if (client_addr == NULL)
         client_addr = new GROUPS;

     do
         res = getwork(client_addr, &rpc_work, &deadlines, NULL);
     while (res != SUCCEEDED);

     opcode = rpc_work.rpc_code;
     if ((opcode == 0) &&
	  (((rpc_work.rpc_buff1 == NULL) && (rpc_work.rpc_length1 == 0)) ||
	 ((rpc_work.rpc_buff2 == NULL) && (rpc_work.rpc_length2 == 0))))
     {
          error_stream << FATAL << "No buffer 1 and/or 2 in rpc structure.\n";
	  ArjunaFatal();
     }
     else
     {
          call1.setBuffer((size_t) rpc_work.rpc_length1, rpc_work.rpc_buff1);
          call2.setBuffer((size_t) rpc_work.rpc_length2, rpc_work.rpc_buff2);
     }

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdoot_rpc::doGetWork()\ncall buffer1 = " << call1  << "\ncall buffer2 = " << call2 << endl;
    debug_stream.unlock();
    
#endif
}

void ServerRajdoot_rpc::sendResult (Int32 errcode, Buffer& result)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdoot_rpc::sendResult (Int32 errcode, Buffer& result)\n" << flush;
    debug_stream.unlock();
    
#endif

     if (! init)
         return;

     RPC rpc_result;

     rpc_result.rpc_code    = errcode;
     rpc_result.rpc_buff1   = (char *)result.buffer();
     rpc_result.rpc_length1 = result.length();
     rpc_result.rpc_buff2   = NULL;
     rpc_result.rpc_length2 = 0;

     if (::sendresult(client_addr, &rpc_result) != SUCCEEDED)
     {
          error_stream << FATAL << "Unable to send result.\n";
	  ArjunaFatal();
     }
}

void ServerRajdoot_rpc::sendResult (Int32 errcode, Buffer& result1, Buffer& result2)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdoot_rpc::sendResult (Int32 errcode, Buffer& result1, Buffer& result2)\n" << flush;
    debug_stream.unlock();
    
#endif

     if (! init)
         return;

     RPC rpc_result;

     rpc_result.rpc_code    = errcode;
     rpc_result.rpc_buff1   = (char *)result1.buffer();
     rpc_result.rpc_length1 = result1.length();
     rpc_result.rpc_buff2   = (char *)result2.buffer();
     rpc_result.rpc_length2 = result2.length();

     if (::sendresult(client_addr, &rpc_result) != SUCCEEDED)
     {
          error_stream << FATAL << "Unable to send result.\n";
	  ArjunaFatal();
     }
}

char* ServerRajdoot_rpc::getServerName()
{
    return ServerName;
}
