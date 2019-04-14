/*
 * Copyright (C) 2000, 2001,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: merant.java,v 1.1 2003/06/19 10:50:31 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.jdbc.brandings;

import com.arjuna.ats.arjuna.logging.tsLogger;
import com.arjuna.ats.arjuna.exceptions.FatalError;

import java.util.*;
import java.sql.*;
import javax.sql.*;
import com.merant.SlExtensionInterface;

import java.sql.SQLException;

/**
 * @message com.arjuna.ats.internal.arjuna.jdbc.brandings.merant_1 ["com.arjuna.ats.internal.arjuna.jdbc.brandings.merant_1] Object to brand is not a merant driver!
 */

public class merant implements Brander
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
		tsLogger.arjLogger.fatal(e.toString());
		    
		throw new FatalError(e.toString());
	    }
	}
	else
	{
	    tsLogger.arjLoggerI18N.fatal("com.arjuna.ats.internal.arjuna.jdbc.brandings.merant_1");
	    throw new FatalError(tsLogger.log_mesg.getString("com.arjuna.ats.arjuna.state.InputBuffer_1"));
	}

	return true;
    }

    private static final String _brandIdentifier = "quemJupiter";

}
