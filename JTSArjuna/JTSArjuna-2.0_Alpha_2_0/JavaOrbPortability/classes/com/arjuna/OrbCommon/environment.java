/*
 * Copyright (C) 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: environment.java,v 1.5.6.2.2.1 1999/11/17 12:37:32 nmcl Exp $
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

public class environment
{

public static final String CORBA_DIAGNOSTICS = "CORBA_DIAGNOSTICS";
public static final String INITIAL_REFERENCES_ROOT = "INITIAL_REFERENCES_ROOT";
public static final String INITIAL_REFERENCES_FILE = "INITIAL_REFERENCES_FILE";
public static final String FILE_DIR = "FILE_DIR";
public static final String SERVER_TIMEOUT = "SERVER_TIMEOUT";
    
};
