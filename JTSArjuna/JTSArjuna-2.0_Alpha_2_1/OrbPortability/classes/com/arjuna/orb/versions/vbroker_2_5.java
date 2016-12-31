/*
 * Copyright (C) 2000,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: vbroker_2_5.java,v 1.1 2000/02/25 14:09:33 nmcl Exp $
 */

package com.arjuna.orb.versions;

import com.arjuna.orb.*;

public class vbroker_2_5 implements ORBData
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
	return 2;
    }
    
public int orbVersionMinor ()
    {
	return 5;
    }    

public int corbaVersionMajor ()
    {
	return 2;
    }
    
public int corbaVersionMinor ()
    {
	return 0;
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
