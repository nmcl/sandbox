/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Configuration.javatmpl,v 1.2.30.2.4.1 2000/12/21 11:21:12 nmcl Exp $
 */

package com.arjuna.CosTransactions;

/**
 * Build-time configuration information for the module.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Configuration.javatmpl,v 1.2.30.2.4.1 2000/12/21 11:21:12 nmcl Exp $
 * @since JTS 1.0.
 */

public class Configuration
{
    
    /**
     * @return the version of the module.
     */

public static final String version ()
    {
	return "4.5.0" ;
    }

}
