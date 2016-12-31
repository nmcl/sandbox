/*
 * Copyright (C) 2000,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: orbacus_4_0.java,v 1.1.2.1 2000/05/02 08:41:17 nmcl Exp $
 */

package com.arjuna.orb.versions;

import com.arjuna.orb.*;

public class orbacus_4_0 implements ORBData
{

public int orbType ()
    {
	return ORBType.O_ORBACUS;
    }

public String orbName ()
    {
	return ORBType.orbacus;
    }

public int orbVersionMajor ()
    {
	return 4;
    }
    
public int orbVersionMinor ()
    {
	return 0;
    }
    
public int corbaVersionMajor ()
    {
	return 2;
    }
    
public int corbaVersionMinor ()
    {
	return 3;
    }

public boolean supportsFilters ()
    {
	return true;
    }

public boolean supportBOA ()
    {
	return false;
    }
    
public boolean supportPOA ()
    {
	return true;
    }
    
};
