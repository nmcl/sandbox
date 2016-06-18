/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PhaseZeroParticipant.java,v 1.3 2003/03/24 11:01:42 nmcl Exp $
 */

package com.arjuna.wst;

/**
 * The PhaseZeroParticipant. This is half of a traditional Synchronization
 * interface.
 */

public interface PhaseZeroParticipant
{

    /**
     * The transaction that the participant was enlisted with is about to
     * complete. The participant may flush cached state (for example) to be
     * managed by a recoverable service prior to the two-phase commit protocol
     * beginning.
     */

    public void phaseZero () throws SystemException;

    /**
     * The coordinator informs the participant that an error has occurred.
     */

    public void error () throws SystemException;
    
}
