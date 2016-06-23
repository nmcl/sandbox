/*
 * Copyright (C) 1997,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StackStarter.cc,v 1.1 1997/09/25 15:32:09 nmcl Exp $
 */

#include <Config/Configure.h>

#include <System/unistd.h>

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef BUFFER_H_

#  include <Common/Buffer.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef CLIENTRPCMAN_H_
#  include <StubGen/ClientRpcMan.h>
#endif

#include <System/fstream.h>

#include "cRBC_stub.h"

cRBC*  process[10];
char*  objectName[10];
int    n_process = 0;

// save object information to the file 'config.dat' and print info to stdout
void printProcessInfo()
{
    int i;
    char* serviceName;
    char* host;
    Uid objUid( NIL_UID);
    Uid serverUid( NIL_UID);
    ProtocolType p;

    ofstream configFile( "config.dat");

    cout << "Object: service name  host     Object uid    server ID   protocol " << endl;
 
    for(i=0; i<n_process; i++) {
      cout.width( 7);
      cout << objectName[i];
      configFile << objectName[i] << endl;
      process[i]->rpcManager().getRpcHandle()->control().getServiceName( serviceName);

      cout.width( 7);
      cout << serviceName;
      configFile << serviceName << endl;

      process[i]->rpcManager().getRpcHandle()->control().getHost( host);
      cout.width( 11);
      cout << host;
      configFile << host << endl;

      process[i]->rpcManager().getRpcHandle()->control().getObjectUid( objUid);
      cout.width( 20);
      cout << objUid;
      configFile << objUid << endl;

      process[i]->rpcManager().getRpcHandle()->control().getServerID( serverUid);
      cout.width( 20);
      cout << serverUid;
      configFile << serverUid << endl;

      process[i]->rpcManager().getRpcHandle()->control().getProtocol( p);
      cout.width( 3);
      cout << p << endl;
      configFile << p << endl;
 
    }
}

Boolean startProcess( char* /*RWCString*/ rpcString)
{
  Boolean ret_val = FALSE;

  // create the remote object
  objectName[ n_process] = rpcString;
  process[ n_process++] =  new cRBC(ret_val);

  if (!ret_val)
    {
      cerr << rpcString << " not created. Constructor error." << endl;
    }
  else
    cout << rpcString << " started." << endl;

  return ret_val;
}

int main (int argc, char** argv)
{
    Boolean res = FALSE;

    res = startProcess( "rbc1");
    cout << "waiting 5 seconds ...\n"; cout.flush(); usleep(5000000);
    res = startProcess( "rbc2");

    printProcessInfo();

    return 0;
}

