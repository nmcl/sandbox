/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: vbroker_4_1.java,v 1.1.2.2 2000/11/14 11:55:18 nmcl Exp $
 */

package com.arjuna.orb.versions;

import com.arjuna.orb.*;

public class vbroker_4_1 implements ORBData
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
