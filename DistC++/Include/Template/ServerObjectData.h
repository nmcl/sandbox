/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerObjectData.h,v 1.1 1997/09/25 15:27:58 nmcl Exp $
 */

#ifndef SERVEROBJECTDATA_H_
#define SERVEROBJECTDATA_H_

#ifndef SVRERRCODE_H_
#  include <StubGen/SvrErrCode.h>
#endif

#ifndef STUBGEN_TYPES_H_
#  include <StubGen/Types.h>
#endif

class ServerConnection;
class RpcBuffer;

template <class T>
class ServerObjectData
{
public:
    ServerObjectData (T* obj, RpcBuffer* buff, Int32 code, const Uid& objID, ServerConnection* conn)
		     : object(obj),
		       work(buff),
		       opcode(code),
		       objectID(objID),
		       connection(conn)
    {
    };
    
    ~ServerObjectData ()
    {
	if (work)
	    delete work;
	if (connection)
	    delete connection;
    }

    T* object;
    RpcBuffer* work;
    Int32 opcode;
    Uid objectID;
    ServerConnection* connection;
};

#endif
