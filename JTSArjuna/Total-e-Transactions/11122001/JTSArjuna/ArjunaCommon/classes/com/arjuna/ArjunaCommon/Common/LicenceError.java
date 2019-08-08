/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK. 
 *
 * $Id: LicenceError.java,v 1.1.2.1.4.1 2000/12/21 11:20:50 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import java.lang.Error;

/**
 * This error is thrown if a licence violation occurs. Further forward
 * progress will be prevented.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: LicenceError.java,v 1.1.2.1.4.1 2000/12/21 11:20:50 nmcl Exp $
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
