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

#ifndef RPOBJSTORE_STUB_H_
#define RPOBJSTORE_STUB_H_

#ifndef STUBGEN_TYPES_H_
#  include <StubGen/Types.h>
#endif

class RpcBuffer;
class ClientRpcManager;


#include <Common/Uid_stub.h>

#include <Common/Buffer_stub.h>

#include <Arjuna/LockMan_stub.h>

#include <Config/Configure.h>
const int RPOBJSTOREUID = 9999;
/* @NoRemote, @NoMarshall */ class StoreManagerList
{
public:
	/* @NoMarshall */ StoreManagerList ( ObjectStore* );
	/* @NoMarshall */ ~StoreManagerList ( );
	ObjectStore* ptr;
	StoreManagerList* next;
};
#include "RPObjStore_stubclass.h"

#endif  /* RPOBJSTORE_STUB_H_ */
