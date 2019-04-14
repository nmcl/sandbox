/*
 * Copyright (C) 2001,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK. 
 *
 * $Id: LicenceError.java,v 1.1 2003/06/19 10:50:18 nmcl Exp $
 */

package com.arjuna.ats.arjuna.exceptions;

import java.lang.Error;

/**
 * This error is thrown if a licence violation occurs. Further forward
 * progress will be prevented.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: LicenceError.java,v 1.1 2003/06/19 10:50:18 nmcl Exp $
 * @since JTS 2.1.
 */

public class LicenceError extends Error
{

public LicenceError ()
    {
	super();
    }

public LicenceError (String s)
    {
	super(s);
    }
}
