/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosActivityCoordinationCurrentImple.javatmpl,v 1.1 2001/07/23 13:44:50 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.Activity;

import com.arjuna.Activity.ActivityCoordinator.ActivityCoordinatorWrapper;
import org.omg.CosActivity.*;
import org.omg.CORBA.*;
import java.util.*;

import java.util.EmptyStackException;
import java.util.NoSuchElementException;
import org.omg.CosActivity.InvalidState;
import org.omg.CosActivity.TimeoutOutOfRange;
import org.omg.CosActivity.NoActivity;
import org.omg.CosActivity.ActivityPending;
import org.omg.CosActivity.ChildContextPending;
import org.omg.CosActivity.SignalSetUnknown;
import org.omg.CosActivity.InvalidToken;
import org.omg.CosActivity.InvalidParentContext;
import org.omg.CosActivity.PropertyGroupUnknown;
import org.omg.CosActivity.AlreadyDestroyed;
import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.SystemException;

public class CosActivityCoordinationCurrentImple extends CosActivityAdministrationCurrentImple implements  org.omg.CosActivityCoordination .CurrentOperations 
{

public Outcome broadcast (String signal_set_name, org.omg.CosActivity.CompletionStatus cs) throws SignalSetUnknown, NoActivity, SystemException
    {
	ActivityCoordinatorWrapper aw = super.currentActivity();
	
	if (aw == null)
	    throw new NoActivity();
	else
	{
	    /*
	     * broadcast is the same as process_signal_set, with the
	     * exception that it can be called from anywhere other
	     * than termination.
	     */

	    return aw.process_signal_set(signal_set_name, cs);
	}
    }

public void add_signal_set (SignalSet signal_set) throws SignalSetAlreadyRegistered, NoActivity, SystemException
      {
	ActivityCoordinatorWrapper aw = super.currentActivity();
	
	if (aw == null)
	    throw new NoActivity();
	else
	    aw.add_signal_set(signal_set);
      }
  
public void remove_signal_set (String signal_set_name) throws SignalSetUnknown, NoActivity, SystemException
    {
	ActivityCoordinatorWrapper aw = super.currentActivity();
	
	if (aw == null)
	    throw new NoActivity();
	else
	    aw.remove_signal_set(signal_set_name);
    }

public void add_action (Action act, String signal_set_name, int priority) throws SignalSetUnknown, NoActivity, SystemException
    {
	ActivityCoordinatorWrapper aw = super.currentActivity();
	
	if (aw == null)
	    throw new NoActivity();
	else
	    aw.add_action(act, signal_set_name, priority);
    }

public void remove_action (Action act) throws ActionNotFound, NoActivity, SystemException
    {
	ActivityCoordinatorWrapper aw = super.currentActivity();
	
	if (aw == null)
	    throw new NoActivity();
	else
	    aw.remove_action(act);
    }
 
public void add_actions (Action[] acts, String signal_set_name, int priority) throws SignalSetUnknown, NoActivity, SystemException
    {
	ActivityCoordinatorWrapper aw = super.currentActivity();
	
	if (aw == null)
	    throw new NoActivity();
	else
	    aw.add_actions(acts, signal_set_name, priority);
    }
 
public Action[] remove_actions (Action[] acts) throws NoActivity, SystemException
    {
	ActivityCoordinatorWrapper aw = super.currentActivity();
	
	if (aw == null)
	    throw new NoActivity();
	else
	    return aw.remove_actions(acts);
    }
 
public void add_global_action (Action act, int priority) throws NoActivity, SystemException
    {
	ActivityCoordinatorWrapper aw = super.currentActivity();
	
	if (aw == null)
	    throw new NoActivity();
	else
	    aw.add_global_action(act, priority);
    }
 
public void remove_global_action (Action act) throws ActionNotFound, NoActivity, SystemException
    {
	ActivityCoordinatorWrapper aw = super.currentActivity();
	
	if (aw == null)
	    throw new NoActivity();
	else
	    aw.remove_global_action(act);
    }
 
public int get_number_registered_actions (String signal_set_name) throws SignalSetUnknown, NoActivity, SystemException
    {
	ActivityCoordinatorWrapper aw = super.currentActivity();
	
	if (aw == null)
	    throw new NoActivity();
	else
	    return aw.get_number_registered_actions(signal_set_name);
    }
 
public Action[] get_actions (String signal_set_name) throws SignalSetUnknown, NoActivity, SystemException
    {
	ActivityCoordinatorWrapper aw = super.currentActivity();
	
	if (aw == null)
	    throw new NoActivity();
	else
	    return aw.get_actions(signal_set_name);
    }
 
};
