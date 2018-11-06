/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TestException.java,v 1.1 1998/06/24 15:07:57 nmcl Exp $
 */

package JavaArjunaLiteTests;

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
