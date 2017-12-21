/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Priorities.java,v 1.1 2003/01/07 10:33:41 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as.coordinator.twophase.common;

/**
 * The typical two-phase coordination protocols have one or two types
 * of participant:
 *
 * (i) the normal participant that takes part in the prepare/commit/rollback.
 * (ii) the pre-two-phase participants (typically known as Synchronizations).
 *
 * Since WSCF allows participants to be prioritised within a coordinator's
 * registered lists or participants, we can use this information to specify
 * whether a participant is a "normal" participant or a Synchronization.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: Priorities.java,v 1.1 2003/01/07 10:33:41 nmcl Exp $
 * @since WSCF 1.0.
 */

public class Priorities
{
  
    /**
     * The participant is a Synchronization.
     */

    public static final int SYNCHRONIZATION = 0;

    /**
     * The participant takes part in the two-phase completion protocol only.
     */

    public static final int PARTICIPANT = 1;

    /**
     * @return the string version of the specified priority.
     */

    public static String stringForm (int res)
    {
	switch (res)
	{
	case SYNCHRONIZATION:
	    return "Priorities.SYNCHRONIZATION";
	case PARTICIPANT:
	    return "Priorities.PARTICIPANT";
	default:
	    return "Unknown - "+res;
	}
    }
	
}
