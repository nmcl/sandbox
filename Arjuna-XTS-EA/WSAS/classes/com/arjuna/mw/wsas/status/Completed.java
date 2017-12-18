/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Completed.java,v 1.1 2002/11/25 10:51:44 nmcl Exp $
 */

package com.arjuna.mw.wsas.status;

/**
 * The activity has completed.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Completed.java,v 1.1 2002/11/25 10:51:44 nmcl Exp $
 * @since 1.0.
 */

public class Completed implements Status
{

    public static Completed instance ()
    {
	return _instance;
    }

    public String toString ()
    {
	return "Status.Completed";
    }

    private Completed ()
    {
    }

    private static final Completed _instance = new Completed();

}
