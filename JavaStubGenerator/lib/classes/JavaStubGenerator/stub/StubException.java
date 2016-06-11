/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StubException.java,v 1.1 1997/07/31 09:13:40 nmcl Exp $
 */

package JavaStubGenerator.stub;

import java.lang.Exception;

public class StubException extends Exception
{

public StubException ()
    {
	super();
    }

public StubException (String s)
    {
	super(s);
    }

}
