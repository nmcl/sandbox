/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActivityManager.java,v 1.1 2002/11/25 10:51:40 nmcl Exp $
 */

package com.arjuna.mw.wsas;

import com.arjuna.mw.wsas.activity.HLS;
import com.arjuna.mw.wsas.activity.OutcomeManager;

import com.arjuna.mw.wsas.exceptions.SystemException;
import com.arjuna.mw.wsas.exceptions.InvalidHLSException;

/**
 * The activity manager is the way in which an HLS can register
 * itself with the activity service. This allows it to be informed
 * of the lifecycle of activities and to augment that lifecyle and
 * associated context.
 *
 * An HLS can be associated with all threads (globally) or with only
 * a specific thread (locally).
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ActivityManager.java,v 1.1 2002/11/25 10:51:40 nmcl Exp $
 * @since 1.0.
 */

public interface ActivityManager
{

    /**
     * Register the specified HLS with the activity service.
     *
     * @param HLS service The HLS to register.
     *
     * @exception InvalidHLSException Thrown if the HLS is invalid in the
     * current execution environment.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void addHLS (HLS service) throws InvalidHLSException, SystemException;

    /**
     * Unregister the specified HLS with the activity service.
     *
     * @param HLS service The HLS to unregister.
     *
     * @exception InvalidHLSException Thrown if the HLS is invalid in the
     * current execution environment.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void removeHLS (HLS service) throws InvalidHLSException, SystemException;

    /**
     * Allows an invoker to obtain a list of all registered HLSs.
     * Elements at the start of the array have higher priority than those at
     * the end.
     *
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the list of HLSs.
     *
     */

    public HLS[] allHighLevelServices () throws SystemException;

    /**
     * When multiple HLSs are registered with the activity service, some
     * means of collating their Outcome responses may be required. An
     * application may provide an OutcomeManager to do this work rather than
     * use the default provided by the system.
     *
     * @param OutcomeManager om The manager to use. If null is specified, then
     * the default manager will be used.
     *
     * @exception SystemException Thrown if any error occurs.
     */

    public void setOutcomeManager (OutcomeManager om) throws SystemException;

    /*
     * When multiple HLSs are registered with the activity service, some
     * means of collating their Outcome responses may be required. This
     * method returns the current manager.
     *
     * @exception SystemException Thrown if any error occurs.
     *
     * @return The manager currently associated with activities.
     */

    public OutcomeManager getOutcomeManager () throws SystemException;
    
}
