/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Synchronization.java,v 1.1.2.1 2000/11/14 12:07:33 nmcl Exp $
 */

import javax.transaction.xa.*;

public class Synchronization implements javax.transaction.Synchronization
{

public void beforeCompletion ()
    {
	System.out.println("beforeCompletion called");
    }

public void afterCompletion (int status)
    {
	System.out.println("afterCompletion called: "+status);
    }
 
}
