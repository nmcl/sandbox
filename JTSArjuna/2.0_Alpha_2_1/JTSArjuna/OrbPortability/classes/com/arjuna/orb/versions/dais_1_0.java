/*
 * Copyright (C) 2000,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: dais_1_0.java,v 1.1 2000/02/25 14:09:28 nmcl Exp $
 */

package com.arjuna.orb.versions;

import com.arjuna.orb.*;

public class dais_1_0 implements ORBData
{

public int orbType ()
    {
	return ORBType.O_DAISJ2;
    }

public String orbName ()
    {
	return ORBType.dais;
    }

public int orbVersionMajor ()
    {
	return 1;
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
	return false;
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

