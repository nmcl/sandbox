/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DefaultIORRecovery.java,v 1.1.2.1.2.1 2000/12/21 11:21:30 nmcl Exp $
 */

package com.arjuna.OrbCommon.recovery;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.orb.ORBInfo;

import java.io.FileNotFoundException;
import java.io.IOException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.SystemException;

/**
 * The default recovery mechanism does the following:
 *
 * (i) if we are a POA based ORB then do nothing since the reference
 *     should have been created as persistent.
 * (ii) call ORBServices with the binding protocols in the following
 *      order: NAME_SERVICE, FILE, NAMED_CONNECT, BIND_CONNECT,
 *             CONFIGURATION_FILE, RESOLVE_INITIAL_REFERENCES
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: DefaultIORRecovery.java,v 1.1.2.1.2.1 2000/12/21 11:21:30 nmcl Exp $
 * @since JTS 2.0.
 */

public class DefaultIORRecovery implements IORRecovery
{

public org.omg.CORBA.Object recover (String name, org.omg.CORBA.Object obj, Object[] params) throws SystemException
    {
	if ((obj == null) || (name == null))
	    throw new BAD_PARAM();

	if (ORBInfo.supportPOA())
	    return obj;
	else
	{
	    int mechanism = ORBServices.NAME_SERVICE;
	    boolean finished = false;
	    org.omg.CORBA.Object nObj = null;
	    
	    while (!finished)
	    {
		try
		{
		    nObj = ORBServices.getService(name, params, mechanism);

		    if (nObj == null)
			throw new BAD_OPERATION();
		    else
			finished = true;
		}
		catch (Exception e)
		{
		    switch (mechanism)
		    {
		    case ORBServices.CONFIGURATION_FILE:
			mechanism = ORBServices.RESOLVE_INITIAL_REFERENCES;
			break;
		    case ORBServices.RESOLVE_INITIAL_REFERENCES:
			finished = true;
			break;
		    case ORBServices.NAME_SERVICE:
			mechanism = ORBServices.FILE;
			break;
		    case ORBServices.FILE:
			mechanism = ORBServices.NAMED_CONNECT;
			break;
		    case ORBServices.BIND_CONNECT:
			mechanism = ORBServices.CONFIGURATION_FILE;
			break;
		    default:
			finished = true;
			break;
		    }
		}
	    }
	    
	    return nObj;
	}
    }

};
