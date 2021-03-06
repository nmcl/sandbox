/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: HeuristicCancel.java,v 1.1 2003/01/07 10:33:46 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.twophase.status;

import com.arjuna.mw.wsas.status.Status;

/**
 * All of the coordiantor's participants cancelled when they were asked to
 * confirm.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: HeuristicCancel.java,v 1.1 2003/01/07 10:33:46 nmcl Exp $
 * @since 1.0.
 */

public class HeuristicCancel implements Status
{

    public static HeuristicCancel instance ()
    {
	return _instance;
    }
    
    public String toString ()
    {
	return "org.w3c.wscf.twophase.status.HeuristicCancel";
    }

    private HeuristicCancel ()
    {
    }

    private static final HeuristicCancel _instance = new HeuristicCancel();
    
}
