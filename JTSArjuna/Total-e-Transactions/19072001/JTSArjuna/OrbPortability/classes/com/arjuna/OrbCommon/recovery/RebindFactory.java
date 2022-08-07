/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RebindFactory.java,v 1.1.2.1.2.1 2000/12/21 11:21:31 nmcl Exp $
 */

package com.arjuna.OrbCommon.recovery;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.OrbCommon.*;

import java.io.FileNotFoundException;
import java.io.IOException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.SystemException;

/**
 * Rebind factory takes a stringified IOR or an object name and
 * returns the relevant object recovery class to use. If none
 * has been provided via java properties, then the default one
 * will be returned.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: RebindFactory.java,v 1.1.2.1.2.1 2000/12/21 11:21:31 nmcl Exp $
 * @since JTS 2.0.
 */

public class RebindFactory
{

    /**
     * Given the name of the object (some application specific identifier) or
     * the object reference, we look up a recovery mechanism for the object.
     * It is valid for either the name or the object reference to be null in
     * this call, but not both.
     */

public static IORRecovery getRecovery (String name, org.omg.CORBA.Object obj) throws SystemException
    {
	if ((name == null) && (obj == null))
	    throw new BAD_PARAM();
	
	String recoveryClassName = null;
	
	if (name != null)
	{
	    recoveryClassName = PropertyManager.getProperty(name);
	    
	    if (recoveryClassName != null)
		return createRecoveryClass(recoveryClassName);
	}
	
	if (obj != null)
	{
	    try
	    {
		String ior = ORBInterface.orb().object_to_string(obj);

		recoveryClassName = PropertyManager.getProperty(ior);
		
		if (recoveryClassName != null)
		    return createRecoveryClass(recoveryClassName);
	    }
	    catch (SystemException e1)
	    {
		ErrorStream.warning().println(e1);

		throw e1;
	    }
	    catch (Exception e2)
	    {
		ErrorStream.warning().println(e2);

		throw new BAD_OPERATION(e2.toString());
	    }
	}
	
	return new DefaultIORRecovery();
    }

private static final IORRecovery createRecoveryClass (String className) throws SystemException
    {
	try
	{
	    Class c = Class.forName(className);

	    return (IORRecovery) c.newInstance();
	}
	catch (Exception e)
	{
	    throw new BAD_OPERATION(e.toString());
	}
    }

};
