/*
 * Automatically Generated by:
 * Arjuna Stub Generator Version 5.5.3 - $Date: 1996/09/10 15:38:44 $
 *
 * Do Not Edit By Hand
 *
 */

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef LOCKDAEMONIF_STUB_H_
#define LOCKDAEMONIF_STUB_H_

#ifndef STUBGEN_TYPES_H_
#  include <StubGen/Types.h>
#endif

class RpcBuffer;
class ClientRpcManager;


#include <System/sys/types.h>

#include <Arjuna/LockMan_stub.h>
static const Int32 DAEMONUID = 666;
class LockTable ;

#ifndef STUB

extern RpcBuffer& operator<< ( RpcBuffer&, const LockTable *);
extern RpcBuffer& operator>> ( RpcBuffer&, LockTable *&);

extern RpcBuffer& operator<< ( RpcBuffer& rpcbuff, const LockTable& topack);
extern RpcBuffer& operator>> ( RpcBuffer& rpcbuff, LockTable& tounpack);
#endif

/* @NoRemote */ class Lock ;

#ifndef STUB

extern RpcBuffer& operator<< ( RpcBuffer&, const Lock *);
extern RpcBuffer& operator>> ( RpcBuffer&, Lock *&);

extern RpcBuffer& operator<< ( RpcBuffer& rpcbuff, const Lock& topack);
extern RpcBuffer& operator>> ( RpcBuffer& rpcbuff, Lock& tounpack);
#endif

/* @NoRemote */ class Uid ;

#ifndef STUB

extern RpcBuffer& operator<< ( RpcBuffer&, const Uid *);
extern RpcBuffer& operator>> ( RpcBuffer&, Uid *&);

extern RpcBuffer& operator<< ( RpcBuffer& rpcbuff, const Uid& topack);
extern RpcBuffer& operator>> ( RpcBuffer& rpcbuff, Uid& tounpack);
#endif

/* @NoRemote */ class LockList ;
#include "LockDaemonIf_stubclass.h"

#endif  /* LOCKDAEMONIF_STUB_H_ */
