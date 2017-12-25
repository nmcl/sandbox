/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Coordinating.java,v 1.1 2002/11/25 10:56:35 nmcl Exp $
 */

package com.arjuna.mw.wscf.status;

import com.arjuna.mw.wsas.status.Status;

/**
 * The Coordinator is coordinating.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Coordinating.java,v 1.1 2002/11/25 10:56:35 nmcl Exp $
 * @since 1.0.
 */

public class Coordinating implements Status
{

    public static Coordinating instance ()
    {
	return _instance;
    }
    
    public String toString ()
    {
	return "Status.Coordinating";
    }

    private Coordinating ()
    {
    }

    private static final Coordinating _instance = new Coordinating();
    
}
