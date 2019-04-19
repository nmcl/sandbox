/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Environment.java,v 1.1 2003/06/19 11:56:12 nmcl Exp $
 */

package com.arjuna.ats.txoj.common;

/**
 * The various runtime configuration options.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Environment.java,v 1.1 2003/06/19 11:56:12 nmcl Exp $
 * @since JTS 1.0.
 */

public class Environment
{
    public static final String PROPERTIES_FILE = "com.arjuna.ats.txoj.common.propertiesFile";
    public static final String LOCKSTORE_DIR = "com.arjuna.ats.txoj.lockstore.lockStoreDir";
    public static final String LOCKSTORE_TYPE = "com.arjuna.ats.txoj.lockstore.lockStoreType";
    public static final String MULTIPLE_LOCKSTORE = "com.arjuna.ats.txoj.lockstore.multipleLockStore";
    public static final String SINGLE_LOCKSTORE = "com.arjuna.ats.txoj.lockstore.singleLockStore";
    public static final String ALLOW_NESTED_LOCKING = "com.arjuna.ats.txoj.lockstore.allowNestedLocking";
}

