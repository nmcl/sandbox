/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: vbroker_4_0.java,v 1.1.2.1 2000/04/25 08:36:27 nmcl Exp $
 */

package com.arjuna.orb;

public class vbroker_4_0 implements ORBData
{

public int orbType ()
    {
	return ORBType.O_VISIBROKER;
    }

public String orbName ()
    {
	return ORBType.vbroker;
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
