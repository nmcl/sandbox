/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TestException.java,v 1.1 2000/02/25 14:08:52 nmcl Exp $
 */

package CosTransactionsTests;

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
