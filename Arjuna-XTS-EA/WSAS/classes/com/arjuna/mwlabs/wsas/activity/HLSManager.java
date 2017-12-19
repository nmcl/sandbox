/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: HLSManager.java,v 1.1 2002/11/25 10:51:45 nmcl Exp $
 */

package com.arjuna.mwlabs.wsas.activity;

import com.arjuna.mw.wsas.ActivityManager;

import com.arjuna.mw.wsas.activity.HLS;
import com.arjuna.mw.wsas.activity.Outcome;
import com.arjuna.mw.wsas.activity.OutcomeManager;

import com.arjuna.mwlabs.wsas.util.HLSWrapper;

import com.arjuna.mw.wsas.exceptions.SystemException;
import com.arjuna.mw.wsas.exceptions.InvalidHLSException;

import com.hp.mwlabs.ts.arjuna.template.OrderedList;
import com.hp.mwlabs.ts.arjuna.template.OrderedListElement;
import com.hp.mwlabs.ts.arjuna.template.OrderedListIterator;

/**
 * The HLS manager is the way in which an HLS can register
 * itself with the activity service. This allows it to be informed
 * of the lifecycle of activities and to augment that lifecyle and
 * associated context.
 *
 * An HLS can be associated with all threads (globally) or with only
 * a specific thread (locally).
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: HLSManager.java,v 1.1 2002/11/25 10:51:45 nmcl Exp $
 * @since 1.0.
 */

public class HLSManager
{

    public static final void addHLS (HLS service) throws InvalidHLSException, SystemException
    {
	if (service == null)
	    throw new InvalidHLSException();
	else
	    _hls.insert(new HLSWrapper(service));
    }

    public static final void removeHLS (HLS service) throws InvalidHLSException, SystemException
    {
	if (service == null)
	    throw new InvalidHLSException();
	else
	{
	    synchronized (_hls)
	    {
		OrderedListIterator iter = new OrderedListIterator(_hls);
		OrderedListElement elem = iter.iterate();
		
		while ((elem != null) && (((HLSWrapper) elem).hls() != service))
		{
		    elem = iter.iterate();
		}
		
		if (elem == null)
		    throw new InvalidHLSException("HLS not found!");
		else
		    _hls.remove(elem);
	    }
	}
    }

    public static final HLS[] allHighLevelServices () throws SystemException
    {
	synchronized (_hls)
	{
	    HLS[] toReturn = new HLS[(int) _hls.size()];
	    OrderedListIterator iter = new OrderedListIterator(_hls);
	    OrderedListElement elem = iter.iterate();
	    int i = 0;
	    
	    while (elem != null)
	    {
		toReturn[i] = ((HLSWrapper) elem).hls();
		i++;
		elem = iter.iterate();
	    }
	    
	    return toReturn;
	}
    }

    public static final void setOutcomeManager (OutcomeManager om) throws SystemException
    {
	synchronized (_outcomeManager)
	{
	    if (om == null)
		om = new OutcomeManagerImple();
	    
	    _outcomeManager = om;
	}
    }

    public static final OutcomeManager getOutcomeManager () throws SystemException
    {
	synchronized (_outcomeManager)
	{
	    return _outcomeManager;
	}
    }

    static final OrderedList HLServices ()
    {
	return _hls;
    }
    
    private static OrderedList _hls = new OrderedList(false); // order decreasing as higher is first
    private static OutcomeManager _outcomeManager = new OutcomeManagerImple();
    
}
