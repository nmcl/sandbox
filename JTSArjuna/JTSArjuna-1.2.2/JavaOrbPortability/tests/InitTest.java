/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InitTest.java,v 1.1 1998/07/06 13:44:40 nmcl Exp $
 */

import com.arjuna.OrbCommon.*;

public class InitTest
{
    
public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
    }
    
};
