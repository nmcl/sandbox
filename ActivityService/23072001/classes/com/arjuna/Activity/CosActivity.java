/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: CosActivity.javatmpl,v 1.1 2001/07/23 13:44:50 nmcl Exp $
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

import com.arjuna.OrbCommon.OAInterface;

public class CosActivity
{

public synchronized static final org.omg.CosActivity.Current get_current ()
    {
	if (_currentRef == null)
	{
	    _currentInstance = new  org.omg.CosActivityCoordination. CurrentPOATie (_currentImple);

	    OAInterface.objectIsReady( _currentInstance ) ;
	    
	    org.omg.CosActivityCoordination.Current tempRef =  org.omg.CosActivityCoordination.CurrentHelper.narrow(OAInterface.corbaReference( _currentInstance )) ;

	    _currentRef =  org.omg.CosActivity.CurrentHelper.narrow(tempRef) ;
	}

	return _currentRef;
    }

public synchronized static final CosActivityCurrentImple current ()
    {
	return _currentImple;
    }

private static CosActivityCoordinationCurrentImple _currentImple = new CosActivityCoordinationCurrentImple();
private	static  org.omg.CosActivityCoordination. CurrentPOATie  _currentInstance = null;
private static org.omg.CosActivity.Current _currentRef = null;

};
