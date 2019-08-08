/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JNSTest.java,v 1.1 2000/02/25 14:03:22 nmcl Exp $
 */

import com.arjuna.ArjunaCommon.ArjunaCommonNames;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Interface.NameService;

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
	NameService nameService = new NameService(ArjunaCommonNames.Implementation_NameService_JNS());

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

		
		
