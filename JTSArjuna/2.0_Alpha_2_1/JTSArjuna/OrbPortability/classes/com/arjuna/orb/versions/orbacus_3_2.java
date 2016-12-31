/*
 * Copyright (C) 2000,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: orbacus_3_2.java,v 1.1 2000/02/25 14:09:30 nmcl Exp $
 */

package com.arjuna.orb.versions;

import com.arjuna.orb.*;

public class orbacus_3_2 implements ORBData
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
	return 3;
    }
    
public int orbVersionMinor ()
    {
	return 2;
    }
    
public int corbaVersionMajor ()
    {
	return 2;
    }
    
public int corbaVersionMinor ()
    {
	return 2;
    }

public boolean supportsFilters ()
    {
	return true;
    }

public boolean supportBOA ()
    {
	return true;
    }
    
public boolean supportPOA ()
    {
	return false;
    }
    
};
