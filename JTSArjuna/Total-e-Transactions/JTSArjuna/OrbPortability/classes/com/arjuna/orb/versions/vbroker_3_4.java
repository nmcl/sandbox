/*
 * Copyright (C) 2000,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: vbroker_3_4.java,v 1.1.2.1 2000/05/31 13:51:49 nmcl Exp $
 */

package com.arjuna.orb.versions;

import com.arjuna.orb.*;

public class vbroker_3_4 implements ORBData
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
	return 3;
    }
    
public int orbVersionMinor ()
    {
	return 4;
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
