/*
 * Copyright (C) 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbixSetup.java,v 1.1.2.1 1999/11/18 13:33:29 nmcl Exp $
 */

package com.arjuna.orb;

import com.arjuna.OrbCommon.ORBInit;
import IE.Iona.OrbixWeb._CORBA;
import IE.Iona.OrbixWeb._OrbixWeb;

public class OrbixSetup
{
    
public OrbixSetup ()
    {
	_CORBA.Orbix.setNoHangup(true);
    }

};
