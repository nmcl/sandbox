/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ClientDebugFilter.java,v 1.1.2.1 2000/11/02 16:54:19 nmcl Exp $
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

public class OTS_ClientDebugFilter implements ClientInterceptor
{
    
public OTS_ClientDebugFilter (org.omg.CORBA.Object obj)
    {
	System.out.println("OTS_ClientDebugFilter created.");
    }

public void prepare_request (RequestHeaderHolder hdr, Closure closure)
    {
    }
    
public org.omg.CORBA.portable.OutputStream send_request (RequestHeader hdr, org.omg.CORBA.portable.OutputStream buf, Closure closure)
    {
	System.out.println("OTS_ClientDebugFilter.send_request called for "+hdr.operation);
	
	closure.object = hdr.operation;

	return null;
    }

public void send_request_failed (RequestHeader hdr, org.omg.CORBA.Environment env,
				 Closure closure)
    {
    }

public void send_request_succeeded (RequestHeader hdr, Closure closure)
    {
    }

public org.omg.CORBA.portable.InputStream receive_reply (ReplyHeader hdr,
							 org.omg.CORBA.portable.InputStream buf,
							 org.omg.CORBA.Environment env,
							 Closure closure)
    {
	String operation = ((closure.object != null) ? (String) closure.object : "Unknown");
	
	System.out.println("OTS_ClientDebugFilter.receive_reply for "+operation);
	
	return null;
    }

public void receive_reply_failed (int req_id, org.omg.CORBA.Environment env, Closure closure)
    {
    }

public void exception_occurred (int req_id, org.omg.CORBA.Environment env, Closure closure)
    {
    }

};
