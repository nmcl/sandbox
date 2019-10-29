/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: listservices.javatmpl,v 1.1.42.1 2001/03/07 15:17:11 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































import com.arjuna.OrbCommon.*;
import org.omg.CosNaming.NamingContextHelper;
import org.omg.CosNaming.NameComponent;
import org.omg.CosNaming.NamingContext;

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
	    NamingContext ncRef =  NamingContextHelper .narrow(objRef);
	    
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
