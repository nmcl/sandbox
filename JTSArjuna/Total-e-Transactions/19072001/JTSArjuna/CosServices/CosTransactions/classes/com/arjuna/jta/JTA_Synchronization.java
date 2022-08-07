/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JTA_Synchronization.javatmpl,v 1.3.2.1.14.3.8.2.4.1 2000/12/21 11:21:23 nmcl Exp $
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































































/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosTranNames.h,v 1.1.42.1 2001/03/20 16:24:47 nmcl Exp $
 */




/*
 * Simple renaming macros for portability.
 *
 * Slightly different from C++ version in that we don't need the
 * typedefs. However, we do need some mapping for OTS exceptions which
 * should now be system exceptions.
 */

/*
 * CosTransaction module exceptions
 */






/*
 * We need to check this. WRONG_TRANSACTION is supposed to
 * be a system exception (in the latest draft of the OTS),
 * but two ORBs have it as a user exception.
 */





/**/




package com.arjuna.jta;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import org.omg.CosTransactions.*;
import javax.transaction.*;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.UNKNOWN;

/**
 * Whenever a synchronization is registered, an instance of this class
 * is used to wrap it.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JTA_Synchronization.javatmpl,v 1.3.2.1.14.3.8.2.4.1 2000/12/21 11:21:23 nmcl Exp $
 * @since JTS 1.2.4.
 */

public class JTA_Synchronization extends  org.omg.CosTransactions . SynchronizationPOA 
{
    
public JTA_Synchronization (javax.transaction.Synchronization ptr)
    {
	_theSynch = ptr;
	_theReference = null;
    }

public void finalize ()
    {
	_theSynch = null;

	try
	{
	    super.finalize();
	}
	catch (Throwable e)
	{
	}
    }

public final org.omg.CosTransactions.Synchronization getSynchronization ()
    {
	if (_theReference == null)
	{
	    OAInterface.objectIsReady( this ) ;
	    
	    _theReference =  org.omg.CosTransactions.SynchronizationHelper.narrow(OAInterface.corbaReference( this )) ;
	}
	
	return _theReference;
    }
 
public void before_completion () throws org.omg.CORBA.SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_Synchronization.before_completion");
	}

	if (_theSynch != null)
	{
	    try
	    {
		_theSynch.beforeCompletion();
	    }
	    catch (Exception e)
	    {
		throw new UNKNOWN();
	    }
	}
	else
	    throw new UNKNOWN();
    }


public void after_completion (org.omg.CosTransactions.Status status) throws org.omg.CORBA.SystemException

    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_Synchronization.after_completion");
	}

	if (_theSynch != null)
	{
	    int s = JTA_Utility.convertStatus(status);

	    try
	    {
		_theSynch.afterCompletion(s);

		if (_theReference != null)
		    OAInterface.shutdownObject( this ) ;
	    }
	    catch (Exception e)
	    {
		if (_theReference != null)
		    OAInterface.shutdownObject( this ) ;

		throw new UNKNOWN(); // should not cause any affect!
	    }
	}
	else
	    throw new UNKNOWN(); // should not cause any affect!
    }

private javax.transaction.Synchronization       _theSynch;
private org.omg.CosTransactions.Synchronization _theReference;

};
