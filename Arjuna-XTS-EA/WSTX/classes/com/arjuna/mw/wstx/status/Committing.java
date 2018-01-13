/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Committing.java,v 1.1 2002/11/25 11:00:53 nmcl Exp $
 */

package com.arjuna.mw.wstx.status;

import com.arjuna.mw.wsas.status.Status;

/**
 * The transaction is in the process of committing.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Committing.java,v 1.1 2002/11/25 11:00:53 nmcl Exp $
 * @since 1.0.
 */

public class Committing implements Status
{

    public static Committing instance ()
    {
	return _instance;
    }
    
    public String toString ()
    {
	return "Status.Committing";
    }

    private Committing ()
    {
    }

    private static final Committing _instance = new Committing();
    
}
