/*
 * Copyright (C) 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Init.java,v 1.1 1999/09/10 13:32:51 nmcl Exp $
 */

package com.arjuna.OrbCommon.Filters.orbix;

import com.arjuna.OrbCommon.ORBInit;

public class Init extends ORBInit
{

public void initialise (String[] params)
    {
	_filter = new BasicThreadFilter();
    }

private static BasicThreadFilter _filter = null;

};
