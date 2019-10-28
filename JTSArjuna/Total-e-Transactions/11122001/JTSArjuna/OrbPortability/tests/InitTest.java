/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: InitTest.java,v 1.1.88.1 2001/08/09 16:15:44 delsworthy Exp $
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
