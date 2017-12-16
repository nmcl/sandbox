/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OutcomeNotificationParticipant.java,v 1.4 2003/03/24 11:01:42 nmcl Exp $
 */

package com.arjuna.wst;

/**
 * The OutcomeNotificationParticipant. Essentially the second half of the
 * traditional Synchronization.
 */

public interface OutcomeNotificationParticipant
{

    /**
     * The transaction committed.
     */

    public void committed () throws SystemException;

    /**
     * The transaction rolled back.
     */

    public void aborted () throws SystemException;

    /**
     * If the participant enquires as to the status of the transaction it was
     * registered with and that transaction is no longer available (has rolled
     * back) then this operation will be invoked by the coordination service.
     */

    public void unknown () throws SystemException;

    /**
     * If the participant enquired as to the status of the transaction it was
     * registered with and an error occurs (e.g., the transaction service is
     * unavailable) then this operation will be invoked.
     */

    public void error () throws SystemException;
    
}
