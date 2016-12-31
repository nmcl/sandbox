/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ActionScope.java,v 1.1 2000/02/25 14:04:36 nmcl Exp $
 */

import com.arjuna.ArjunaLite.Atomic.*;
import com.arjuna.ArjunaCommon.Common.*;

public class ActionScope
{
    
public static void main (String[] args)
    {
	test();

	System.gc();
	
	System.runFinalization();
    }

public static void test ()
    {
	AtomicAction A = new AtomicAction();
	
	A.begin();

	A = null;

	System.gc();	
    }

};
