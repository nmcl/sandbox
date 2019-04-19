/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TestException.java,v 1.2 2001/11/20 12:17:14 nmcl Exp $
 */

package com.hp.mwtests.ts.txoj;

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
