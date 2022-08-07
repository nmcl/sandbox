/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BadAction.java,v 1.1.4.1 2000/10/20 09:17:39 nmcl Exp $
 */

import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCommon.Common.*;

public class BadAction
{
    
public static void main (String[] args)
    {
	AtomicAction A = new AtomicAction();
	AtomicAction B = new AtomicAction();

	System.out.println("Starting top-level action.");
	
	A.begin();

	System.out.println(A);

	System.out.println("\nStarting nested action.");

	B.begin();

	System.out.println(B);

	System.out.println("\nCommitting top-level action. This should fail.");
	
	A.commit();

	System.out.println("\nCommitting nested action. This should fail.");

	B.commit();

	BasicAction current = BasicAction.Current();

	System.out.println("\nCurrent action is " + current);
    }
    
};
