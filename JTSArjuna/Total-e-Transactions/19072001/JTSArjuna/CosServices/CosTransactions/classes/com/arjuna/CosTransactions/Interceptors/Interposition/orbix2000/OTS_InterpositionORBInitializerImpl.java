/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_InterpositionORBInitializerImpl.java,v 1.1.2.2.6.1.2.2 2001/02/14 10:44:43 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interceptors.Interposition.orbix2000;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosTransactions.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.ArjunaCore.Thread.ThreadActionData;
import org.omg.CosTransactions.*;
import org.omg.CORBA.*;
import org.omg.PortableInterceptor.*; 
import org.omg.PortableInterceptor.ORBInitInfoPackage.*; 
import org.omg.IOP.*;
import org.omg.IOP.CodecFactoryPackage.*;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CosTransactions.Unavailable;

public class OTS_InterpositionORBInitializerImpl extends LocalObject implements ORBInitializer
{

public OTS_InterpositionORBInitializerImpl ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_InterpositionORBInitializerImpl ()");
	}

	/*
	 * Register the thread-setup object so that ArjunaCore can be
	 * used raw.
	 */

	ThreadActionData.addSetup(new OTS_InterpositionThreadSetup());
    }
  
public void pre_init (ORBInitInfo init_info)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_InterpositionORBInitializer.pre_init ()");
	}

	/*
	 * These value should be part of the standard.
	 */

	int localSlot = init_info.allocate_slot_id();
	int receivedSlot = init_info.allocate_slot_id();

	com.arjuna.CosTransactions.OTS.setLocalSlotId(localSlot);
	com.arjuna.CosTransactions.OTS.setReceivedSlotId(receivedSlot);
	
	/*
	 * Get the CDR codec; used for encoding/decoding the service
	 * context and IOR components.
	 */

	Codec cdr_codec = null;

	try
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						   (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_InterpositionORBInitializerImpl - getting reference to ENCODING_CDR_ENCAPS codec");
	    }

	    Encoding cdr_encoding = new Encoding(ENCODING_CDR_ENCAPS.value, (byte)1, (byte)2);

	    cdr_codec = init_info.codec_factory().create_codec(cdr_encoding);
	}
	catch (UnknownEncoding ex)
	{
	    ErrorStream.fatal().println("OTS_InterpositionORBInitializerImpl - getting ENCODING_CDR_ENCAPS codec");

	    ex.printStackTrace();

	    System.exit(1);
	}
    
	/*
	 * Register client interceptor to propogate the context.
	 */

	try
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						   (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_InterpositionORBInitializerImpl - registering ClientRequestInterceptor");
	    }

	    ClientRequestInterceptor client_interceptor = new OTS_InterpositionClientRequestInterceptorImpl(localSlot, cdr_codec);

	    init_info.add_client_request_interceptor(client_interceptor);
	}
	catch (DuplicateName ex)
	{
	    ErrorStream.fatal().println("OTS_InterpositionORBInitializerImpl - registering ClientRequestInterceptor");

	    ex.printStackTrace();

	    System.exit(1);
	}
  
	/*
	 * Register a server interceptor to receive the context.
	 */

	try
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						   (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_InterpositionORBInitializerImpl - registering ServerRequestInterceptor");
	    }

	    ServerRequestInterceptor server_interceptor = new OTS_InterpositionServerRequestInterceptorImpl(receivedSlot, cdr_codec);

	    init_info.add_server_request_interceptor(server_interceptor);
	}
	catch (DuplicateName ex)
	{
	    ErrorStream.fatal().println("OTS_InterpositionORBInitializerImpl - registering ServerRequestInterceptor");

	    ex.printStackTrace();

	    System.exit(1);
	}
    }
  
public void post_init (ORBInitInfo init_info)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_InterpositionORBInitializerImpl.post_init ()");
	}

	// nothing to do
    }
  
}
