/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: fooSignalSet.javatmpl,v 1.1 2001/07/23 13:45:05 nmcl Exp $
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
































































package com.arjuna.ActivityTests;

import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.Activity.*;
import com.arjuna.Activity.SignalSets.SystemDefined;
import org.omg.CosActivity.*;
import org.omg.CORBA.*;
import java.io.*;

import org.omg.CosActivity.ActionError;
import org.omg.CosActivity.AlreadyDestroyed;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.SystemException;

public class fooSignalSet extends  org.omg.CosActivity . SignalSetPOA 
{

public fooSignalSet ()
    {
	_destroyed = false;
	_reference = null;
	_finished = false;
    }
 
public final synchronized SignalSet getReference ()
    {
	if (_reference == null)
   	{
	    OAInterface.objectIsReady( this ) ;
	    
	    _reference =  org.omg.CosActivity.SignalSetHelper.narrow(OAInterface.corbaReference( this )) ;
	}

	return _reference;
    }

public String signal_set_name () throws SystemException
    {
	return "com.arjuna.ActivityTests.fooSignalSet";
    }
 
public Signal get_signal (BooleanHolder lastSignal) throws SystemException
    {
	if (!_finished)
	{
	    Signal s = new Signal("fooSignal", signal_set_name(), null);

	    _finished = lastSignal.value = true;
	    
	    return s;
	}
	else
	    return null;
    }
 
public Outcome get_outcome () throws SignalSetActive, SystemException
    {
	if (!_finished)
	    throw new SignalSetActive();
	else
	    return new Outcome("Dummy", ORBInterface.orb().create_any());
    }

public boolean set_response (Outcome response, BooleanHolder nextSignal) throws SignalSetInactive, SystemException
    {
	return true;
    }
 
public void set_completion_status (org.omg.CosActivity.CompletionStatus cs) throws SystemException
    {
    }
 
public org.omg.CosActivity.CompletionStatus get_completion_status () throws SystemException
    {
	return org.omg.CosActivity.CompletionStatus.CompletionStatusSuccess;
    }

public void set_activity_coordinator (ActivityCoordinator coord) throws SignalSetActive, SystemException
    {
    }

public synchronized void destroy () throws AlreadyDestroyed, SystemException
    {
	if (_destroyed)
	    throw new AlreadyDestroyed();
	else
	{
	    _destroyed = true;
	    _reference = null;
	    OAInterface.shutdownObject( this ) ;
        }
    }

private boolean   _destroyed;
private SignalSet _reference;
private boolean   _finished;
 
};
