/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: InitTest.java,v 1.1 2000/02/25 14:10:03 nmcl Exp $
 */

import com.arjuna.OrbCommon.*;

public class InitTest
{
    
public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initOA();
    }
    
};
