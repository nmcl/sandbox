/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: JNSTest.java,v 1.1 1998/11/18 09:01:33 nmcl Exp $
 */

import com.arjuna.JavaGandiva.JavaGandivaNames;
import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaGandiva.Interface.NameService;

import java.io.IOException;

/*
 * To test:
 *
 * java -DLONG=#5 -DSTRING=^"foobar" JNSTest
 */

public class JNSTest
{
    
public static void main (String[] args)
    {
	NameService nameService = new NameService(JavaGandivaNames.Implementation_NameService_JNS());

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

		
		
