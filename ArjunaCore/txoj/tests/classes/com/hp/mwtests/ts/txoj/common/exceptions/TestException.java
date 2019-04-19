/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TestException.java,v 1.1 2002/05/29 11:06:17 rbegg Exp $
 */

package com.hp.mwtests.ts.txoj.common.exceptions;

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
