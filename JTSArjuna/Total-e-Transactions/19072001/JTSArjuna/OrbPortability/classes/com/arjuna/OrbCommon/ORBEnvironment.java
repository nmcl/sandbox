/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBEnvironment.java,v 1.1.28.1.2.2.2.1.2.1.28.1.4.2 2001/07/12 10:25:15 jcoleman Exp $
 */

package com.arjuna.OrbCommon;

import com.arjuna.ArjunaCommon.Common.EnvironmentManager;
import java.util.Hashtable;

/**
 * Various property variables which can be set to
 * achieve different effects.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ORBEnvironment.java,v 1.1.28.1.2.2.2.1.2.1.28.1.4.2 2001/07/12 10:25:15 jcoleman Exp $
 * @since JTS 1.0.
 * @see com.arjuna.ArjunaCommon.Common.EnvironmentManager
 */

/*
 * These names had better be unique!
 */

public class ORBEnvironment implements com.arjuna.ArjunaCommon.Common.EnvironmentManager
{

public static final String CORBA_DIAGNOSTICS = "CORBA_DIAGNOSTICS";
public static final String INITIAL_REFERENCES_ROOT = "INITIAL_REFERENCES_ROOT";
public static final String INITIAL_REFERENCES_FILE = "INITIAL_REFERENCES_FILE";
public static final String FILE_DIR = "FILE_DIR";
public static final String RESOLVE_SERVICE = "RESOLVE_SERVICE";
public static final String SERVER_TIMEOUT = "SERVER_TIMEOUT";
public static final String EVENT_HANDLER = "EVENT_HANDLER";
public static final String ORB_IMPLEMENTATION = "ORB_IMPLEMENTATION";
public static final String OA_IMPLEMENTATION = "OA_IMPLEMENTATION";
  
public String getPropertyRange (String propertyName)
    {
	if (propertyName.equals(CORBA_DIAGNOSTICS))
	    return _ranges[0];
	if (propertyName.equals(INITIAL_REFERENCES_ROOT))
	    return _ranges[1];
	if (propertyName.equals(INITIAL_REFERENCES_FILE))
	    return _ranges[2];
	if (propertyName.equals(FILE_DIR))
	    return _ranges[3];
	if (propertyName.equals(RESOLVE_SERVICE))
	    return _ranges[4];
	if (propertyName.equals(SERVER_TIMEOUT))
	    return _ranges[5];
	if (propertyName.equals(EVENT_HANDLER))
	    return _ranges[6];
	if (propertyName.equals(ORB_IMPLEMENTATION))
	    return _ranges[7];
	if (propertyName.equals(OA_IMPLEMENTATION))
	    return _ranges[8];

	return EnvironmentManager.INVALID;
    }

    /*
     * Do not re-order!
     */

private String[] _ranges = { "int:0x0...0xff", "string:#notnull#", "string:#notnull#", "string:#notnull#", "int:"+ORBServices.CONFIGURATION_FILE+"..."+ORBServices.BIND_CONNECT, "int:0...#max#", "string:#notnull#", "string:#notnull#", "string:#notnull#" };
  
}
