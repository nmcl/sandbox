/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: merant.java,v 1.1.2.2.20.3.8.1 2001/04/06 13:35:22 nmcl Exp $
 */

package com.arjuna.JDBC2.drivers.brandings;

import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.ArjunaCommon.Common.FatalError;
import com.arjuna.JDBC2.JDBC2Branding;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import com.merant.SlExtensionInterface;

import java.sql.SQLException;

public class merant implements JDBC2Branding
{

public boolean brand (Object theConnection)
    {
	if (theConnection instanceof SlExtensionInterface)
	{
	    SlExtensionInterface slCon = (SlExtensionInterface) theConnection;
		
	    try
	    {
		slCon.setOemId(merant._brandIdentifier);
	    }
	    catch (Exception e)
	    {
		ErrorStream.fatal().println(e.toString());
		    
		throw new FatalError(e.toString());
	    }
	}
	else
	{
	    ErrorStream.fatal().println("Object to brand is not a merant driver!");

	    throw new FatalError("Object to brand is not a merant driver!");
	}

	return true;
    }

private static final String _brandIdentifier = "quemJupiter";

};
