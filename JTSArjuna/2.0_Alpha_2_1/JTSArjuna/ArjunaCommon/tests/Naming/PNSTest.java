/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: PNSTest.java,v 1.1 2000/02/25 14:03:22 nmcl Exp $
 */

import com.arjuna.ArjunaCommon.ArjunaCommonNames;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Interface.NameService;

import java.io.IOException;

public class PNSTest
{
    
public static void main (String[] args)
    {
	try
	{
	    String longAttr = new String("LONG");
	    String stringAttr = new String("STRING");
	    String objAttr = new String("OBJECTNAME");
	    String uidAttr = new String("UID");
	    
	    long l = 12345;
	    Uid u = new Uid();
	    String s = new String("test");

	    ObjectName o1 = new ObjectName("PNS:o1");
	    ObjectName o2 = new ObjectName("PNS:o2");
	    
	    System.out.println("Adding <string, "+s+"> to o1");
	    o1.setStringAttribute(stringAttr, s);

	    System.out.println("\nAdding <long, "+l+"> to o2");
	    o2.setLongAttribute(longAttr, l);
	    
	    System.out.println("Adding <uid, "+u+"> to o2");
	    o2.setUidAttribute(uidAttr, u);

	    System.out.println("\nAdding <object name, o2> to o1");
	    o1.setObjectNameAttribute(objAttr, o2);

	    System.out.println("\nNow extracting string from o1");
	    System.out.println("Extracted: "+o1.getStringAttribute(stringAttr));

	    System.out.println("\nNow extracting object name from o1");
	    ObjectName o3 = o1.getObjectNameAttribute(objAttr);
	    System.out.println("Extracted: "+o3);

	    System.out.println("\nNow extracting long from o2");
	    System.out.println("Extracted: "+o3.getLongAttribute(longAttr));

	    System.out.println("\nNow extracting uid from o2");
	    Uid id = o3.getUidAttribute(uidAttr);
	    System.out.println("Extracted: "+id);

	    if (id.notEquals(u))
		System.out.println("Uid error.");

	    ObjectName o4 = ObjectName.uniqueObjectName("PNS");
	    
	    System.out.println("\nUnique object name: "+o4);
	}
	catch (IOException e)
	{
	    System.out.println(e);
	}
    }
    
}

		
		
