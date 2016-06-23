

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef CLIENTRPCMAN_H_
#  include <StubGen/ClientRpcMan.h>
#endif

#include "cRBC_stub.h"

#define COCIFC_SERVER
#ifndef OCIFC_H_
#  include "cOCifc_stub.h"
#endif

#include <System/iostream.h>
#include <assert.h>

cOCifc::cOCifc (Boolean& res)
{
  status = 0;
    
  res = TRUE;
}

cOCifc::~cOCifc ()
{
}

Boolean cOCifc::init()
{
    Boolean ret_val = FALSE;
    ClientRpc* cRPC = (ClientRpc*) 0;
    ClientRpcManager* rpcC = (ClientRpcManager*) 0;
    Uid u;

    cRPC = new ClientRpc( "RBC");
    (cRPC->control()).setProtocol(STREAM);
    (cRPC->control()).setServerID(u);      // shared server id.
    rpcC = new ClientRpcManager(cRPC);
    // at this point, the compiler raises an error since rbc is a LocalcRBC while 'new cRBC' is a RemotecRBC !
    rbc = new cRBC(ret_val, rpcC);

    return ret_val;
}

int cOCifc::rbcStatus()
{
  return rbc->getStatus();
}

int cOCifc::getStatus()
{
  return status;
}

void cOCifc::setStatus( int s)
{
  status = s;
}

void cOCifc::crash()
{
  assert( 1==2);

}
