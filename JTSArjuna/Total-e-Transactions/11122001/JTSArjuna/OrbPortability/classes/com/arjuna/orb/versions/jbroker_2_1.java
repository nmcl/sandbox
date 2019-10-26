/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: jbroker_2_1.java,v 1.1.28.1 2000/11/14 11:55:18 nmcl Exp $
 */

package com.arjuna.orb.versions;

import com.arjuna.orb.*;

public class jbroker_2_1 implements ORBData
{

public int orbType ()
    {
	return ORBType.O_JBROKER;
    }

public String orbName ()
    {
	return ORBType.jbroker;
    }

public int orbVersionMajor ()
    {
	return 2;
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
    
};
