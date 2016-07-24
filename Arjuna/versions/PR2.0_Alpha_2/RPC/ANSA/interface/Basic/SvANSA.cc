/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: SvANSA.cc,v 1.1 1993/11/03 14:38:53 nmcl Exp $
 *
 * This class provides C++ interface to the server side of the
 * ANSA rpc protocol.
 *
 */

#if defined(DEBUG) && !defined(DEBUG_H)
#  include <Common/Debug.h>
#endif

#ifndef STRING_H_
#   include <System/string.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef SVANSA_H_
#  include "RPC/ANSA/SvANSA.h"
#endif

extern "C"
{
#include "../../src/tRPC.h"
extern void rpc_getwork( RPCBuff * );
extern int rpc_sendresult( RPCBuff );
}

static const char RCSid[] = "$Id: SvANSA.cc,v 1.1 1993/11/03 14:38:53 nmcl Exp $";

ServerANSA_rpc::ServerANSA_rpc()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS <<  FAC_BASIC_RPC <<  VIS_PUBLIC;
    debug_stream << "ServerANSA_rpc::ServerANSA_rpc()\n" << flush;
#endif
}


ServerANSA_rpc::~ServerANSA_rpc()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ServerANSA_rpc::~ServerANSA_rpc()\n" << flush;
#endif
}

int ServerANSA_rpc::Initialise(int argc, char *argv[])
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ServerANSA_rpc::Initialise(int argc, char *argv[])\n";
    debug_stream << "    argc = " << argc << "\n";
    for(int a = 0; a < argc; a++)
	debug_stream << "   argv[" << a << "] = " << argv[a] << "\n" << flush;
#endif

    ServerName = strdup(argv[0]);

    return TRUE;
}

void ServerANSA_rpc::GetWork(long& opcode, Buffer& call)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ServerANSA_rpc::GetWork(long& opcode, Buffer& call)\n" << flush;
#endif

    RPCBuff work;

    work.Opcode = 0;
    work.Buffer1.length = 0;
    work.Buffer1.data   = NULL;
    work.Buffer2.length = 0;
    work.Buffer2.data   = NULL;
    rpc_getwork(&work);
    opcode = work.Opcode;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ServerANSA_rpc::Getwork() Data received\n";
    debug_stream << "                             Opcode = " << opcode << "\n";
    debug_stream << "                             Buffer1.length = " << work.Buffer1.length << "\n" << flush;
#endif

    if ((opcode == 0) && (work.Buffer1.data == NULL) &&
	(work.Buffer1.length == 0))
    {
	error_stream << FATAL << "No buffer 1 in rpc structure.\n";
	ArjunaFatal();
    }
    else
    {
	call.setBuffer((size_t) work.Buffer1.length, (char *) work.Buffer1.data);
    }
}

void ServerANSA_rpc::GetWork(long& opcode, Buffer& call1, Buffer& call2)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ServerANSA_rpc::GetWork(long& opcode, Buffer& call1, Buffer& call2)\n" << flush;
#endif

    RPCBuff work;

    work.Opcode = 0;
    work.Buffer1.length = 0;
    work.Buffer1.data   = NULL;
    work.Buffer2.length = 0;
    work.Buffer2.data   = NULL;
    rpc_getwork(&work);
    opcode = work.Opcode;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ServerANSA_rpc::Getwork() Data received\n";
    debug_stream << "                              Opcode = " << opcode << "\n";
    debug_stream << "                              Buffer1.length = " << work.Buffer1.length << "\n";
    debug_stream << "                              Buffer2.length = " << work.Buffer2.length << "\n" << flush;
#endif

    if ((opcode == 0) &&
	(((work.Buffer1.data == NULL) && (work.Buffer1.length == 0)) ||
	 ((work.Buffer2.data == NULL) && (work.Buffer2.length == 0))))

    {
	error_stream << FATAL << "No buffer 1 and/or 2 in rpc structure.\n";
	ArjunaFatal();
    }
    else
    {
	call1.setBuffer((size_t) work.Buffer1.length, (char *) work.Buffer1.data);
	call2.setBuffer((size_t) work.Buffer2.length, (char *) work.Buffer2.data);
    }
}

void ServerANSA_rpc::SendResult(long errcode, Buffer& result)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ServerANSA_rpc::SendResult(long errcode, Buffer& result)\n" << flush;
#endif

    RPCBuff send;

    send.Opcode = errcode;
    send.Buffer1.length = result.length();
    send.Buffer1.data   = (ansa_Octet *) result.buffer();
    send.Buffer2.length = 0;
    send.Buffer2.data   = NULL;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "                   Opcode = " << send.Opcode << "\n";
    debug_stream << "                   Buffer1.length = " << send.Buffer1.length << "\n" << flush;
#endif

    if (rpc_sendresult(send) != 1)
    {
	error_stream << FATAL << "Unable to send result.\n";
	ArjunaFatal();
    }
}

void ServerANSA_rpc::SendResult(long errcode, Buffer& result1, Buffer& result2)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ServerANSA_rpc::SendResult(long errcode, Buffer& result1, Buffer& result2)\n" << flush;
#endif

    RPCBuff send;

    send.Opcode = errcode;
    send.Buffer1.length = result1.length();
    send.Buffer1.data   = (ansa_Octet *) result1.buffer();
    send.Buffer2.length = result2.length();
    send.Buffer2.data   = (ansa_Octet *) result2.buffer();

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "                   Opcode = " << send.Opcode << "\n";
    debug_stream << "                   Buffer1.length = " << send.Buffer1.length << "\n";
    debug_stream << "                   Buffer2.length = " << send.Buffer2.length << "\n" << flush;
#endif

    if (rpc_sendresult(send) != 1)
    {
	error_stream << FATAL << "Unable to send result.\n";
	ArjunaFatal();
    }
}
