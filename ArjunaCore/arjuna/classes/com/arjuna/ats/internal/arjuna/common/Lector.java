/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Lector.java,v 1.1 2003/06/19 10:50:29 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.common;

import com.arjuna.ats.arjuna.common.*;
import com.hp.mwlabs.lic.LicManager;

/**
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Lector.java,v 1.1 2003/06/19 10:50:29 nmcl Exp $
 * @since JTS 2.1.
 */

public class Lector
{

	public static final synchronized boolean confirm (int value)
	{
		if (_licmgr == null)
			_licmgr = (new ArjunaLicenceControl()).toString();
		return(LicManager.confirm(_licmgr, value));
	}

	private static String _licmgr = null;
}
