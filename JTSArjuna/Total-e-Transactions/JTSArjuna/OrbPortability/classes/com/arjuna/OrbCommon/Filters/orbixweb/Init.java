/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Init.java,v 1.1.2.1.32.1 2000/12/21 11:21:29 nmcl Exp $
 */

package com.arjuna.OrbCommon.Filters.orbixweb;

import com.arjuna.OrbCommon.ORBAttribute;

public class Init extends ORBAttribute
{

public void initialise (String[] params)
    {
	_filter = new BasicThreadFilter();
    }

private static BasicThreadFilter _filter = null;

};
