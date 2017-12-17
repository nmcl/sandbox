/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OutcomeManager.java,v 1.2 2003/04/04 14:59:51 nmcl Exp $
 */

package com.arjuna.mw.wsas.activity;

import com.arjuna.mw.wsas.completionstatus.CompletionStatus;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * When multiple HLSs are registered with the activity service, some
 * means of collating their Outcome responses may be required.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: OutcomeManager.java,v 1.2 2003/04/04 14:59:51 nmcl Exp $
 * @since 1.0.
 */

public interface OutcomeManager
{

    /**
     * Given the current (composite) Outcome and the Outcome just obtained
     * from an HLS, the OutcomeManager may return a new Outcome.
     *
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the Oucome or null.
     */

    public Outcome processOutcome (Outcome current, Outcome next) throws SystemException;

}
