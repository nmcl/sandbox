/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStoreType.java,v 1.3 2003/06/26 14:02:45 jcoleman Exp $
 */

package com.arjuna.ats.arjuna.objectstore;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.gandiva.ClassName;
import java.io.*;

import com.arjuna.ats.arjuna.logging.tsLogger;

/**
 * The various types of object store implementations that are
 * available.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ObjectStoreType.java,v 1.3 2003/06/26 14:02:45 jcoleman Exp $
 * @since JTS 1.0.
 */

public class ObjectStoreType
{

    /*
     * Do not re-order these!
     */
    
public static final int ACTION = 0;
public static final int ACTIONLOG = 1;
public static final int SHADOWING = 2;
public static final int FRAGMENTED = 3;
public static final int VOLATILE = 4;
public static final int HASHED = 5;
public static final int REPLICATED = 6;
public static final int MAPPED = 7;
public static final int SINGLETYPEMAPPED = 8;
public static final int USER_DEF_0 = 9;
public static final int USER_DEF_1 = 10;
public static final int USER_DEF_2 = 11;
public static final int USER_DEF_3 = 12;
public static final int USER_DEF_4 = 13;
public static final int SHADOWNOFILELOCK = 14;
public static final int JDBC = 15;
public static final int JDBC_ACTION = 16;
public static final int HASHED_ACTION = 17;
public static final int CACHED = 18;

    /**
     * @return the <code>ClassName</code> for this object store value.
     * @see com.arjuna.ats.arjuna.gandiva.ClassName
     */

public final static ClassName typeToClassName (int rt)
    {
	switch (rt)
	{
	case ACTION:
	    return ArjunaNames.Implementation_ObjectStore_ActionStore();
	case ACTIONLOG:
	    return ArjunaNames.Implementation_ObjectStore_ActionLogStore();
	case SHADOWING:
	    return ArjunaNames.Implementation_ObjectStore_ShadowingStore();
	case FRAGMENTED:
	    return ArjunaNames.Implementation_ObjectStore_FragmentedStore();
	case VOLATILE:
	    return ArjunaNames.Implementation_ObjectStore_VolatileStore();
	case HASHED:
	    return ArjunaNames.Implementation_ObjectStore_HashedStore();
	case REPLICATED:
	    return ArjunaNames.Implementation_ObjectStore_ReplicatedStore();
	case MAPPED:
	    return ArjunaNames.Implementation_ObjectStore_MappedStore();
	case SINGLETYPEMAPPED:
	    return ArjunaNames.Implementation_ObjectStore_SingleTypeMappedStore();
	case USER_DEF_0:
	    return ArjunaNames.Implementation_ObjectStore_UserDef0Store();
	case USER_DEF_1:
	    return ArjunaNames.Implementation_ObjectStore_UserDef1Store();
	case USER_DEF_2:
	    return ArjunaNames.Implementation_ObjectStore_UserDef2Store();
	case USER_DEF_3:
	    return ArjunaNames.Implementation_ObjectStore_UserDef3Store();
	case USER_DEF_4:
	    return ArjunaNames.Implementation_ObjectStore_UserDef4Store();
	case SHADOWNOFILELOCK:
	    return ArjunaNames.Implementation_ObjectStore_ShadowNoFileLockStore();
	case JDBC:
	    return ArjunaNames.Implementation_ObjectStore_JDBCStore();
	case JDBC_ACTION:
	    return ArjunaNames.Implementation_ObjectStore_JDBCActionStore();
	case HASHED_ACTION:
	    return ArjunaNames.Implementation_ObjectStore_HashedActionStore();
	case CACHED:
	    return ArjunaNames.Implementation_ObjectStore_CacheStore();
	default:
	    return null;
	}
    }

    /**
     * @return the <code>int</code> value for this object store.
     * <code>ClassName</code>.
     * @see com.arjuna.ats.arjuna.gandiva.ClassName
     *
     * @message com.arjuna.ats.arjuna.objectstore.ObjectStoreType_1 [com.arjuna.ats.arjuna.objectstore.ObjectStoreType_1] -  unknown store: {0}
     * @message com.arjuna.ats.arjuna.objectstore.ObjectStoreType_2 [com.arjuna.ats.arjuna.objectstore.ObjectStoreType_2] -  unknown store: 
     */
    
public final static int classNameToType (ClassName c)
    {
	if (c.equals(ArjunaNames.Implementation_ObjectStore_ActionStore()))
	    return ACTION;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_ActionLogStore()))
	    return ACTIONLOG;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_ShadowingStore()))
	    return SHADOWING;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_FragmentedStore()))
	    return FRAGMENTED;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_VolatileStore()))
	    return VOLATILE;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_HashedStore()))
	    return HASHED;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_ReplicatedStore()))
	    return REPLICATED;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_MappedStore()))
	    return MAPPED;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_SingleTypeMappedStore()))
	    return SINGLETYPEMAPPED;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_ShadowNoFileLockStore()))
	    return SHADOWNOFILELOCK;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_JDBCStore()))
	    return JDBC;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_JDBCActionStore()))
	    return JDBC_ACTION;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_HashedActionStore()))
	    return HASHED_ACTION;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_CacheStore()))
	    return CACHED;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_UserDef0Store()))
	    return USER_DEF_0;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_UserDef1Store()))
	    return USER_DEF_1;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_UserDef2Store()))
	    return USER_DEF_2;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_UserDef3Store()))
	    return USER_DEF_3;
	if (c.equals(ArjunaNames.Implementation_ObjectStore_UserDef4Store()))
	    return USER_DEF_4;

	if (tsLogger.arjLoggerI18N.isWarnEnabled())
	{
	    tsLogger.arjLoggerI18N.warn("ObjectStoreType_1", new Object[]{c});
	}

	throw new com.arjuna.ats.arjuna.exceptions.FatalError(tsLogger.log_mesg.getString("com.arjuna.ats.arjuna.objectstore.ObjectStoreType_2")+c);
    }

    /**
     * Print on the specified <code>PrintWriter</code> a string
     * representation of the object store value.
     */

public final static void print (PrintWriter strm, int rt)
    {
	ClassName c = typeToClassName(rt);
	
	strm.print(c);

	c = null;
    }

    /**
     * @return <code>true</code> if the value is valid, <code>false</code> 
     * otherwise.
     */

public static final boolean valid (int rt)
    {
	if (typeToClassName(rt) != null)
	    return true;
	else
	    return false;
    }
    

 

}











