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

import com.arjuna.JavaArjuna.ClassLib.*;
import com.arjuna.JavaArjuna.Common.*;

public class ActionScope
{
    
public static void main (String[] args)
    {
	test();

	System.gc();
	
	System.runFinalization();

	System.out.println("Ending without explicitly terminating transaction - watch for warnings!");
    }

public static void test ()
    {
	AtomicAction A = new AtomicAction();
	
	A.begin();

	A = null;

	System.gc();	
    }

};
