/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CancelOnly.java,v 1.1 2003/01/07 10:33:45 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.twophase.status;

import com.arjuna.mw.wsas.status.Status;

/**
 * The status of the coordination is such that it will eventually cancel.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: CancelOnly.java,v 1.1 2003/01/07 10:33:45 nmcl Exp $
 * @since 1.0.
 */

public class CancelOnly implements Status
{

    public static CancelOnly instance ()
    {
	return _instance;
    }
    
    public String toString ()
    {
	return "org.w3c.wscf.twophase.status.CancelOnly";
    }

    private CancelOnly ()
    {
    }

    private static final CancelOnly _instance = new CancelOnly();
    
}
