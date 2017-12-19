/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActivityManagerImple.java,v 1.1 2002/11/25 10:51:45 nmcl Exp $
 */

package com.arjuna.mwlabs.wsas;

import com.arjuna.mw.wsas.ActivityManager;

import com.arjuna.mw.wsas.activity.HLS;
import com.arjuna.mw.wsas.activity.OutcomeManager;

import com.arjuna.mwlabs.wsas.activity.HLSManager;

import com.arjuna.mwlabs.wsas.util.HLSWrapper;

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
 * @version $Id: ActivityManagerImple.java,v 1.1 2002/11/25 10:51:45 nmcl Exp $
 * @since 1.0.
 */

public class ActivityManagerImple implements ActivityManager
{

    public ActivityManagerImple ()
    {
    }
    
    public void addHLS (HLS service) throws InvalidHLSException, SystemException
    {
	HLSManager.addHLS(service);
    }

    public void removeHLS (HLS service) throws InvalidHLSException, SystemException
    {
	HLSManager.removeHLS(service);
    }

    public HLS[] allHighLevelServices () throws SystemException
    {
	return HLSManager.allHighLevelServices();
    }

    public void setOutcomeManager (OutcomeManager om) throws SystemException
    {
	HLSManager.setOutcomeManager(om);
    }

    public OutcomeManager getOutcomeManager () throws SystemException
    {
	return HLSManager.getOutcomeManager();
    }
    
}
