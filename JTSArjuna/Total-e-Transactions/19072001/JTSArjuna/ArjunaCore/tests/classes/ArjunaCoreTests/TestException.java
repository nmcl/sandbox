/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TestException.java,v 1.1.4.1 2000/10/20 09:17:49 nmcl Exp $
 */

package ArjunaCoreTests;

import java.lang.Exception;

public class TestException extends Exception
{

public TestException ()
    {
	super();
    }

public TestException (String s)
    {
	super(s);
    }

}
