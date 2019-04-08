/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActionManager.java,v 1.1 2001/07/23 13:44:52 nmcl Exp $
 */

package com.arjuna.Activity.Actions;

import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.Activity.*;
import org.omg.CosActivity.*;
import org.omg.CORBA.*;
import java.io.PrintWriter;

import org.omg.CosActivity.ActionError;
import org.omg.CosActivity.AlreadyDestroyed;
import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.SystemException;

/*
 * To allow for different quality of services for the coordinator-to-action
 * interactions, we go through an ActionManager (one instance per coordinator).
 * This can dynamically select an implementation based upon application
 * requirements.
 */

public class ActionManager
{

public ActionManager ()
    {
	_theActionManager = null;

	try
	{
	    Class c = Class.forName(_actionManagerClass);
	    
	    _theActionManager = (ActionManagerIntf) c.newInstance();
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	}
    }
    
public Outcome process_signal (Action act, Signal sig) throws ActionError, SystemException
    {
	if (_theActionManager == null)
	    throw new BAD_OPERATION();

	if ((act == null) || (sig == null))
	    throw new BAD_PARAM();

	return _theActionManager.process_signal(act, sig);
    }
 
public void destroy (Action act) throws AlreadyDestroyed, SystemException
    {
	if (_theActionManager == null)
	    throw new BAD_OPERATION();

	if (act == null)
	    throw new BAD_PARAM();

	_theActionManager.destroy(act);
    }

private ActionManagerIntf _theActionManager;

private static String _actionManagerClass = null;
private static String _defaultActionManager = "com.arjuna.Activity.Actions.DefaultActionManager";
    
    static
    {
	String actionManager = PropertyManager.getProperty(com.arjuna.Activity.Environment.ACTION_MANAGER);

	if (actionManager != null)
	    _actionManagerClass = actionManager;
	else
	    _actionManagerClass = actionManager;
    }
	
};
