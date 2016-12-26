/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: merant.java,v 1.1.2.1.12.1 2001/04/06 13:35:22 nmcl Exp $
 */

package com.arjuna.JDBC.drivers.brandings;

import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.ArjunaCommon.Common.FatalError;
import com.arjuna.JDBC.JDBCBranding;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import com.merant.SlExtensionInterface;

import java.sql.SQLException;

public class merant implements JDBCBranding
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

}
