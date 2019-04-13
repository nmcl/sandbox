/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DummyAction.javatmpl,v 1.1 2001/07/23 13:45:05 nmcl Exp $
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
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.Activity.*;
import com.arjuna.Activity.SignalSets.SystemDefined;
import org.omg.CosActivity.*;
import java.io.*;

import org.omg.CosActivity.ActionError;
import org.omg.CosActivity.AlreadyDestroyed;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.SystemException;

public class DummyAction extends  org.omg.CosActivity . ActionPOA 
{

public static final int NO_EXCEPTION = 0;
public static final int ACTION_ERROR = 1;
public static final int UNKNOWN_EXCEPTION = 2;

public DummyAction (int actionType, int index)
    {
	_destroyed = false;
	_reference = null;
	_actionType = actionType;
	_index = index;
	_theUid = new Uid();
    }

public final synchronized Action getReference ()
    {
	if (_reference == null)
   	{
	    OAInterface.objectIsReady( this ) ;
	    
	    _reference =  org.omg.CosActivity.ActionHelper.narrow(OAInterface.corbaReference( this )) ;
	}

	return _reference;
    }

public Outcome process_signal (Signal sig) throws ActionError, SystemException
    {
	if (sig == null)
	    throw new BAD_PARAM();

	System.out.println("\nAction "+_index+" received signal: < "+sig.signal_set_name+", "+sig.signal_name+" >");

	if (SystemDefined.member(sig.signal_set_name))
	{
	    ActivityInformation ai = ActivityInformationHelper.extract(sig.application_specific_data);

	    PrintWriter strm = new PrintWriter(System.out);
	    
	    Utility.printActivityInformation(ai, strm);

	    strm.println("\n");
	    strm.flush();
	}

	switch (_actionType)
   	{
	case ACTION_ERROR:
	    {
		System.out.println("\tAction throwing ActionError");
	    
		throw new ActionError();
	    }
	case UNKNOWN_EXCEPTION:
	    {
		System.out.println("\tAction throwing UNKNOWN SystemException");
		
		throw new UNKNOWN();
	    }
	default:
	    // do nothing
	}

	Outcome o = new Outcome();

	o.outcome_name = "DummyOutcome";
	o.application_specific_data = ORBInterface.orb().create_any();
	o.application_specific_data.insert_short((short) 0);

	return o;
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

private boolean _destroyed;
private Action  _reference;
private int     _actionType;
private int     _index;
private Uid     _theUid;
 
};
