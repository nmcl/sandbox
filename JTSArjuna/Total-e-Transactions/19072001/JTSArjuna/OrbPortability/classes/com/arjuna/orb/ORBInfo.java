/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBInfo.javatmpl,v 1.1.2.1.6.2.22.1.2.2.4.1.4.1 2001/04/06 13:35:22 nmcl Exp $
 */

package com.arjuna.orb;

import com.arjuna.ArjunaCommon.Common.ErrorStream;

/**
 * This class queries the ORB specific ORBData object for information
 * about the ORB.
 */

public class ORBInfo
{

public static final int BOA = 0;
public static final int POA = 1;

public static final int orbType ()
    {
	return _theData.orbType();
    }

public static final String orbName ()
    {
	return _theData.orbName();
    }

public static final int orbVersionMajor ()
    {
	return _theData.orbVersionMajor();
    }
    
public static final int orbVersionMinor ()
    {
	return _theData.orbVersionMinor();
    }
    
public static final int corbaVersionMajor ()
    {
	return _theData.corbaVersionMajor();
    }
    
public static final int corbaVersionMinor ()
    {
	return _theData.corbaVersionMinor();
    }

public static final boolean supportsFilters ()
    {
	return _theData.supportsFilters();
    }

public static final boolean supportBOA ()
    {
	return _theData.supportBOA();
    }
    
public static final boolean supportPOA ()
    {
	return _theData.supportPOA();
    }

public static final ORBData orbData ()
    {
	return _theData;
    }
    
private static ORBData _theData = null;

    static
    {
	String className = null;







	className = "com.arjuna.orb.versions.orbix2000_";







	className = className+ "1_2" ;

	try
	{
	    Class c = Class.forName(className);
	
	    _theData = (ORBData) c.newInstance();
	}
	catch (Exception e)
	{
	    ErrorStream.stream(ErrorStream.FATAL).println("ORBInfo ORB specific class creation failed with: "+e);

	    throw new ExceptionInInitializerError("ORBInfo ORB specific class creation failed with: "+e);
	}
    }
    
};
