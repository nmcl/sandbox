/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK. 
 *
 * $Id: FatalError.java,v 1.1.2.1.4.1 2000/12/21 11:20:49 nmcl Exp $
 */

package com.arjuna.JavaArjuna.Common;

import java.lang.Error;

/**
 * This error is thrown if a fatal error occurs which would prevent further
 * progress without possibly compromising consistency.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: FatalError.java,v 1.1.2.1.4.1 2000/12/21 11:20:49 nmcl Exp $
 * @since JTS 2.1.
 */

public class FatalError extends Error
{

public FatalError ()
    {
	super();
    }

public FatalError (String s)
    {
	super(s);
    }
}
