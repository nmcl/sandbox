/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: DemoPreInit.java,v 1.1.88.1 2001/08/09 16:15:44 delsworthy Exp $
 */

import com.arjuna.OrbCommon.*;

public class DemoPreInit
{

public DemoPreInit ()
    {
	System.out.println("DemoPreInit called.");

	if (ORBInterface.orb() == null)
	    System.out.println("The orb has not been initialised.");
	else
	    System.out.println("Error - the orb has been initialised.");
    }

};
