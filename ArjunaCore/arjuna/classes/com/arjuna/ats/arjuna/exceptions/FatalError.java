/*
 * Copyright (C) 2001,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK. 
 *
 * $Id: FatalError.java,v 1.1 2003/06/19 10:50:18 nmcl Exp $
 */

package com.arjuna.ats.arjuna.exceptions;

import java.lang.Error;

/**
 * This error is thrown if a fatal error occurs which would prevent further
 * progress without possibly compromising consistency.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: FatalError.java,v 1.1 2003/06/19 10:50:18 nmcl Exp $
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
