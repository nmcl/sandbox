package com.hp.mwtests.ts.arjuna.naming;

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JNSTest.java,v 1.2 2003/06/19 10:51:19 nmcl Exp $
 */

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.gandiva.nameservice.NameService;

import java.io.IOException;

/*
 * To test:
 *
 * java -DLONG=#5 -DSTRING=^"foobar" com.hp.mwtests.ts.arjuna.naming.JNSTest
 */

public class JNSTest
{
    
public static void main (String[] args)
    {
	NameService nameService = new NameService(ArjunaNames.Implementation_NameService_JNS());

	try
	{
	    String longAttr = new String("LONG");
	    long lvalue = nameService.getLongAttribute(null, longAttr);

	    System.out.println("Long value: "+lvalue);

	    String stringAttr = new String("STRING");
	    String svalue = nameService.getStringAttribute(null, stringAttr);

	    System.out.println("String value: "+svalue);
	}
	catch (IOException e)
	{
	    System.out.println(e);
	}
    }
    
}

		
		
