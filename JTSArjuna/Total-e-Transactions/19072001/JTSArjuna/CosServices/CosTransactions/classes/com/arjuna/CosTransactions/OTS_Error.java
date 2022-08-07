/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_Error.java,v 1.1.2.1.4.1 2000/12/21 11:21:15 nmcl Exp $
 */

package com.arjuna.CosTransactions;

import java.lang.Error;

/**
 * Error that may be thrown during transaction processing.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_Error.java,v 1.1.2.1.4.1 2000/12/21 11:21:15 nmcl Exp $
 * @since JTS 2.0.
 */

public class OTS_Error extends Error
{

public OTS_Error ()
    {
	super();
    }

public OTS_Error (String s)
    {
	super(s);
    }
}
