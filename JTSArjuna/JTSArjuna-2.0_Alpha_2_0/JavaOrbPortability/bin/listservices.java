/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: listservices.javatmpl,v 1.3 1998/07/06 13:31:45 nmcl Exp $
 */

/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability.h,v 1.4 1998/06/30 16:34:10 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability_krc.h,v 1.3 1999/09/10 13:49:11 nmcl Exp $
 */













































import com.arjuna.OrbCommon.*;

import org.omg.CORBA.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;


import org.omg.CORBA.SystemException;
import org.omg.CosNaming.NamingContextPackage.InvalidName;

public class listservices
{
    
public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);

	String[] services = ORBInterface.orb().list_initial_services();

	if (services == null)
	{
	    System.out.println("There are no initial services.");
	}
	else
	{
	    System.out.println("Initial services are:\n");
	    
	    for (int i = 0; i < services.length; i++)
	    {
		System.out.println(services[i]);
	    }
	}

	try
	{
	    org.omg.CORBA.Object objRef = ORBInterface.orb().resolve_initial_references(ORBServices.nameService);
	    NamingContext ncRef = NamingContextHelper.narrow(objRef);
	    
	    // bind the Object Reference in Naming
		    
	    NameComponent nc = new NameComponent(ORBServices.transactionService, "");
	    NameComponent path[] = {nc};

	    org.omg.CORBA.Object o = ncRef.resolve(path);

	    if (o != null)
		System.out.println("resolved transaction manager.");
	    else
		System.out.println("failed to resolve transaction manager.");
	}
	catch (Exception e)
	{
	    System.err.println(e);
	}
    }

};
