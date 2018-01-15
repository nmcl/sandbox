/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TxState.java,v 1.1 2003/04/04 15:00:56 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.coordinator;

import com.arjuna.mw.wsas.activity.*;

import java.util.Hashtable;

public class TxState
{

    public static final int COMMITTED = 0;
    public static final int ABORTED = 1;
    public static final int UNKNOWN = 2;
    public static final int HEURISTIC = 3;
    
    public static final void setState (ActivityHierarchy hier, int state)
    {
	_states.put(hier, new Integer(state));
    }
    
    public static final int getState (ActivityHierarchy hier)
    {
	Integer i = (Integer) _states.get(hier);
	
	if (i != null)
	    return i.intValue();
	else
	    return TxState.UNKNOWN;
    }
    
    public static final void removeState (ActivityHierarchy hier)
    {
	_states.remove(hier);
    }

    private static Hashtable _states = new Hashtable();
    
}
