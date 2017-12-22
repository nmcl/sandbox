/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: HeuristicConfirm.java,v 1.1 2003/01/07 10:33:46 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.twophase.status;

import com.arjuna.mw.wsas.status.Status;

/**
 * All of the coordinator's participants confirmed when they were asked to
 * cancel.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: HeuristicConfirm.java,v 1.1 2003/01/07 10:33:46 nmcl Exp $
 * @since 1.0.
 */

public class HeuristicConfirm implements Status
{

    public static HeuristicConfirm instance ()
    {
	return _instance;
    }
    
    public String toString ()
    {
	return "org.w3c.wscf.twophase.status.HeuristicConfirm";
    }

    private HeuristicConfirm ()
    {
    }

    private static final HeuristicConfirm _instance = new HeuristicConfirm();
    
}
