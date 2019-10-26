/*
 * Copyright (C) 2000,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: orbixweb_3_1.java,v 1.1.2.1 2000/05/02 08:41:18 nmcl Exp $
 */

package com.arjuna.orb.versions;

import com.arjuna.orb.*;

public class orbixweb_3_1 implements ORBData
{

public int orbType ()
    {
	return ORBType.O_ORBIXWEB;
    }

public String orbName ()
    {
	return ORBType.orbixweb;
    }

public int orbVersionMajor ()
    {
	return 3;
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
