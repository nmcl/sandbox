/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBEnvironment.java,v 1.1 2000/02/25 14:09:17 nmcl Exp $
 */

package com.arjuna.OrbCommon;

import java.util.Hashtable;

/**
 * Various property variables which can be set to
 * achieve different effects.
 */

/*
 * These names had better be unique!
 */

public class ORBEnvironment
{

public static final String CORBA_DIAGNOSTICS = "CORBA_DIAGNOSTICS";
public static final String INITIAL_REFERENCES_ROOT = "INITIAL_REFERENCES_ROOT";
public static final String INITIAL_REFERENCES_FILE = "INITIAL_REFERENCES_FILE";
public static final String FILE_DIR = "FILE_DIR";
public static final String RESOLVE_SERVICE = "RESOLVE_SERVICE";
public static final String SERVER_TIMEOUT = "SERVER_TIMEOUT";
    
};
