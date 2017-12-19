/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: HLSWrapper.java,v 1.1 2002/11/25 10:51:46 nmcl Exp $
 */

package com.arjuna.mwlabs.wsas.util;

import com.arjuna.mw.wsas.activity.HLS;

import com.hp.mwlabs.ts.arjuna.template.OrderedListElement;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: HLSWrapper.java,v 1.1 2002/11/25 10:51:46 nmcl Exp $
 * @since 1.0.
 */

public class HLSWrapper implements OrderedListElement
{

    public HLSWrapper (HLS hls)
    {
	_theHLS = hls;
    }
    
    /**
     * Are the two entries equal?
     */

    public boolean equals (OrderedListElement e)
    {
	if (e instanceof HLSWrapper)
	{
	    try
	    {
		return (boolean) (((HLSWrapper) e).hls().priority() == hls().priority());
	    }
	    catch (SystemException ex)
	    {
		return false;
	    }
	}
	else
	    return false;
    }

    /**
     * Is the current entry less than the one in the parameter?
     */

    public boolean lessThan (OrderedListElement e)
    {
	if (e instanceof HLSWrapper)
	{
	    try
	    {
		return (boolean) (((HLSWrapper) e).hls().priority() < hls().priority());
	    }
	    catch (SystemException ex)
	    {
		return false;
	    }
	}
	else
	    return false;
    }	

    /**
     * Is the current entry greater than the one in the parameter?
     */
 
    public boolean greaterThan (OrderedListElement e)
    {
	if (e instanceof HLSWrapper)
	{
	    try
	    {
		return (boolean) (((HLSWrapper) e).hls().priority() > hls().priority());
	    }
	    catch (SystemException ex)
	    {
		return false;
	    }
	}
	else
	    return false;
    }		

    public final HLS hls ()
    {
	return _theHLS;
    }
    
    private HLS _theHLS;
    
}

