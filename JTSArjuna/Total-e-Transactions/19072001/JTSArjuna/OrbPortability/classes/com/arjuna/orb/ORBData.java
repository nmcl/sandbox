/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ORBData.java,v 1.1.28.2.4.1 2000/12/21 11:21:32 nmcl Exp $
 */

package com.arjuna.orb;

/**
 * ORB specific data objects implement this interface to provide
 * runtime information about the ORB.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ORBData.java,v 1.1.28.2.4.1 2000/12/21 11:21:32 nmcl Exp $
 * @since JTS 2.0.
 */

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
