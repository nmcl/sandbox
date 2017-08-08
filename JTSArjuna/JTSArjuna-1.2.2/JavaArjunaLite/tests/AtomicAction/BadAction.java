/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BadAction.java,v 1.1 1998/11/12 09:39:04 nmcl Exp $
 */

import com.arjuna.JavaArjunaLite.Atomic.*;
import com.arjuna.JavaGandiva.Common.*;

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
