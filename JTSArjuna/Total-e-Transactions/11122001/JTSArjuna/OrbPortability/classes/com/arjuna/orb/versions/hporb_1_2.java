/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: hporb_1_2.java,v 1.1.2.1 2002/06/17 13:00:09 nmcl Exp $
 */

package com.arjuna.orb.versions;

import com.arjuna.orb.*;

public class hporb_1_2 implements ORBData
{

public int orbType ()
    {
	return ORBType.O_HPORB;
    }

public String orbName ()
    {
	return ORBType.hporb;
    }

public int orbVersionMajor ()
    {
	return 1;
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
	return 4;
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
	return true;
    }

}
