/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerDebugFilter.java,v 1.1.2.1 2000/11/02 16:54:20 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Debug.vbroker;

import com.arjuna.OrbCommon.ORBInterface;

import com.visigenic.vbroker.interceptor.*;
import com.visigenic.vbroker.IOP.*;
import com.visigenic.vbroker.GIOP.*;
import com.visigenic.vbroker.orb.*;
import org.omg.CORBA.portable.*;
import org.omg.CORBA.BOA;
import org.omg.CORBA.CompletionStatus;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;

public class OTS_ServerDebugFilter implements ServerInterceptor
{
    
public OTS_ServerDebugFilter ()
    {
	System.out.println("OTS_ServerDebugFilter");
    }

public IOR locate (int req_id, byte[] object_key, Closure closure)
    {
        return null;
    }

public void locate_succeeded (int req_id, Closure closure)
    {
    }

public void locate_forwarded (int req_id, IORHolder forward_ior, Closure closure)
    {
    }

public IOR locate_failed (int req_id, byte[] object_key, Closure closure)
    {
        return null;
    }

public InputStream receive_request (RequestHeader hdr, org.omg.CORBA.ObjectHolder target, org.omg.CORBA.portable.InputStream buf, Closure closure)
    {
	System.out.println("OTS_ServerDebugFilter.receive_request called for "+hdr.operation);

	return null;
    }

public void prepare_reply (RequestHeader hdr, ReplyHeaderHolder reply,
			   org.omg.CORBA.Object target, Closure closure)
    {
    }

public org.omg.CORBA.portable.OutputStream send_reply (RequestHeader reqHdr, ReplyHeader hdr,
						       org.omg.CORBA.Object target,
						       org.omg.CORBA.portable.OutputStream buf,
						       org.omg.CORBA.Environment env,
						       Closure closure)
    {
	System.out.println("OTS_ServerDebugFilter.send_reply for "+reqHdr.operation);
	
	return null;
    }

public void send_reply_failed (RequestHeader reqHdr, ReplyHeader replyHdr, org.omg.CORBA.Object target, org.omg.CORBA.Environment env, Closure closure)
    {
    }

public void request_completed (RequestHeader reqHdr, org.omg.CORBA.Object target, Closure closure)
    {
    }

public void shutdown (com.visigenic.vbroker.interceptor.ServerInterceptorPackage.ShutdownReason reason)
    {
    }

public void exception_occurred (RequestHeader reqHdr, org.omg.CORBA.Environment env, Closure closure)
    {
    }

};
