/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JDBCRecoveryModule.java,v 1.1.2.3.2.1.2.1.4.2 2001/01/26 16:40:49 pfurniss Exp $
 */

package com.arjuna.JDBC.Recovery;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.JDBC.JDBCResource;
import com.arjuna.JDBC.JDBCEnvironment;
import com.arjuna.CosRecovery.RecoveryModule;
import java.util.*;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import java.io.IOException;

/*
 * JDBC 1.0 recovery is extremely simple since it is not possible
 * to reconnect to a JDBC connection after a recovery! All we can
 * do is find any heuristic resources and report on them.
 */

public class JDBCRecoveryModule implements RecoveryModule
{

public final static String FORGET = "JDBC_FORGET";

public JDBCRecoveryModule ()
    {
	String doForget = PropertyManager.getProperty(JDBCRecoveryModule.FORGET);

	_doForget = false;

	if (doForget != null)
	{
	    if (doForget.equals("YES"))
		_doForget = true;
	}
	_objStore = new ObjectStore();
    }

public void periodicWorkFirstPass ()
    {
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_RECOVERY_NORMAL,
						 "JDBCRecoveryModule - first pass");
	}
	_uids = new InputObjectState();

	try
	{
	    if (!_objStore.allObjUids(_type, _uids))
	    {
		ErrorStream.warning().println(id()+" could not get all object Uids.");

		return;
	    }
	}
	catch (ObjectStoreException e)
	{
	    ErrorStream.warning().println(e);

	    return;
	}

    }

public void periodicWorkSecondPass ()
    {
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_RECOVERY_NORMAL,
						 "JDBCRecoveryModule - second pass");
	}

	Uid theUid = new Uid(Uid.nullUid());

	do
	{
	    try
	    {
		theUid.unpack(_uids);

		if (theUid.notEquals(Uid.nullUid()))
		{
		    try
		    {
			if (_objStore.currentState(theUid, _type) != ObjectStore.OS_UNKNOWN)
			{
			    JDBCRecoveryResource record = new JDBCRecoveryResource(theUid);

			    if (!record.recover())
				ErrorStream.warning().println("JDBC failed to recover "+theUid);
			    else
			    {
				if (_doForget)
				{
				    try
				    {
					record.forget();
				    }
				    catch (Exception e)
				    {
					ErrorStream.warning().println("forget on "+theUid+" raised: "+e);
				    }
				}
			    }
			}
		    }
		    catch (Exception e1)
		    {
		    }
		}
	    }
	    catch (IOException e2)
	    {
		theUid = Uid.nullUid();
	    }

	} while (theUid.notEquals(Uid.nullUid()));
    }

public String id ()
    {
	return "JDBCRecoveryModule";
    }

private static String _type = JDBCResource.typeName();

private ObjectStore _objStore;

private InputObjectState _uids ;

private boolean _doForget;

private static int _backoffPeriod = 0;

}
