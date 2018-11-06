/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActionScope.java,v 1.1.2.1 1999/01/28 19:00:19 nmcl Exp $
 */

import com.arjuna.JavaArjunaLite.Atomic.*;
import com.arjuna.JavaGandiva.Common.*;

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
