/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: orbix2000_1_1.java,v 1.1.2.1 2000/11/23 15:26:10 nmcl Exp $
 */

package com.arjuna.orb.versions;

import com.arjuna.orb.*;

public class orbix2000_1_1 implements ORBData
{

public int orbType ()
    {
	return ORBType.O_ORBIX2000;
    }

public String orbName ()
    {
	return ORBType.orbix2000;
    }

public int orbVersionMajor ()
    {
	return 1;
    }
    
public int orbVersionMinor ()
    {
	return 1;
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

}
