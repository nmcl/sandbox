/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: jacorb_1_2.java,v 1.1.2.1 2001/02/01 15:31:47 nmcl Exp $
 */

package com.arjuna.orb.versions;

import com.arjuna.orb.*;

public class jacorb_1_2 implements ORBData
{

public int orbType ()
    {
	return ORBType.O_JACORB;
    }

public String orbName ()
    {
	return ORBType.jacorb;
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
