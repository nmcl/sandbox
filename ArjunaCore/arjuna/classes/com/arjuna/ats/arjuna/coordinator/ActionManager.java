/*
 * Copyright (C) 2001,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ActionManager.java,v 1.1 2003/06/19 10:50:15 nmcl Exp $
 */

package com.arjuna.ats.arjuna.coordinator;

import com.arjuna.ats.arjuna.common.Uid;

import java.util.Hashtable;

/*
 * @author Mark Little (mark_little@hp.com)
 * @version $Id: ActionManager.java,v 1.1 2003/06/19 10:50:15 nmcl Exp $
 * @since JTS 3.0
 */

public class ActionManager
{

    public static final ActionManager manager ()
    {
	return _theManager;
    }
    
    public void put (BasicAction act)
    {
	_allActions.put(act.get_uid(), act);
    }
    
    public BasicAction get (Uid id)
    {
	return (BasicAction) _allActions.get(id);
    }
    
    public void remove (Uid id)
    {
 	_allActions.remove(id);
    }
    
    private ActionManager ()
    {
    }
    
    private static ActionManager _theManager = new ActionManager();
    private static Hashtable     _allActions = new Hashtable();

}
