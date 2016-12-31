/*
 * Copyright (C) 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ORBData.java,v 1.1 2000/02/25 14:09:26 nmcl Exp $
 */

package com.arjuna.orb;

public interface ORBData
{

public int orbType ();
public String orbName ();

public int orbVersionMajor ();
public int orbVersionMinor ();
 
public int corbaVersionMajor ();
public int corbaVersionMinor ();

public boolean supportsFilters ();

public boolean supportBOA ();
public boolean supportPOA ();
 
};
