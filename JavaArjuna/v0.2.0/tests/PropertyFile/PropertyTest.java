/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AtomicTest.java,v 1.1 1998/11/12 09:39:07 nmcl Exp $
 */

import com.arjuna.JavaArjuna.Common.*;

public class PropertyTest
{
    
public static void main (String[] args)
    {
	Utility.loadProperties();

	String dir = System.getProperty("LOCALOSROOT");

	if ("MyStore".equals(dir))
	    System.out.println("Success");
	else
	    System.out.println("Failure");
    }
    
}
