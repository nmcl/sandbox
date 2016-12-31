/*
 * Copyright (C) 2000,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: sunsjdk_1_2.java,v 1.1 2000/02/25 14:09:32 nmcl Exp $
 */

package com.arjuna.orb.versions;

import com.arjuna.orb.*;

public class sunsjdk_1_2 implements ORBData
{

public int orbType ()
    {
	return ORBType.O_SUNSJDK;
    }

public String orbName ()
    {
	return ORBType.sun;
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
	return 2;
    }

public boolean supportsFilters ()
    {
	return false;
    }

public boolean supportBOA ()
    {
	return false;
    }
    
public boolean supportPOA ()
    {
	return false;
    }
    
};
