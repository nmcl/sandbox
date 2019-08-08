/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStoreType.java,v 1.1.4.1.2.3.2.2.2.1 2000/12/21 11:21:10 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.ObjectStore;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaCommon.Common.ErrorStream;
import java.io.*;

/**
 * The various types of object store implementations that are
 * available.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ObjectStoreType.java,v 1.1.4.1.2.3.2.2.2.1 2000/12/21 11:21:10 nmcl Exp $
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

    /**
     * @return the <code>ClassName</code> for this object store value.
     * @see com.arjuna.ArjunaCommon.Common.ClassName
     */

public final static ClassName typeToClassName (int rt)
    {
	switch (rt)
	{
	case ACTION:
	    return ArjunaCoreNames.Implementation_ObjectStore_ActionStore();
	case ACTIONLOG:
	    return ArjunaCoreNames.Implementation_ObjectStore_ActionLogStore();
	case SHADOWING:
	    return ArjunaCoreNames.Implementation_ObjectStore_ShadowingStore();
	case FRAGMENTED:
	    return ArjunaCoreNames.Implementation_ObjectStore_FragmentedStore();
	case VOLATILE:
	    return ArjunaCoreNames.Implementation_ObjectStore_VolatileStore();
	case HASHED:
	    return ArjunaCoreNames.Implementation_ObjectStore_HashedStore();
	case REPLICATED:
	    return ArjunaCoreNames.Implementation_ObjectStore_ReplicatedStore();
	case MAPPED:
	    return ArjunaCoreNames.Implementation_ObjectStore_MappedStore();
	case SINGLETYPEMAPPED:
	    return ArjunaCoreNames.Implementation_ObjectStore_SingleTypeMappedStore();
	case USER_DEF_0:
	    return ArjunaCoreNames.Implementation_ObjectStore_UserDef0Store();
	case USER_DEF_1:
	    return ArjunaCoreNames.Implementation_ObjectStore_UserDef1Store();
	case USER_DEF_2:
	    return ArjunaCoreNames.Implementation_ObjectStore_UserDef2Store();
	case USER_DEF_3:
	    return ArjunaCoreNames.Implementation_ObjectStore_UserDef3Store();
	case USER_DEF_4:
	    return ArjunaCoreNames.Implementation_ObjectStore_UserDef4Store();
	case SHADOWNOFILELOCK:
	    return ArjunaCoreNames.Implementation_ObjectStore_ShadowNoFileLockStore();
	case JDBC:
	    return ArjunaCoreNames.Implementation_ObjectStore_JDBCStore();
	case JDBC_ACTION:
	    return ArjunaCoreNames.Implementation_ObjectStore_JDBCActionStore();
	case HASHED_ACTION:
	    return ArjunaCoreNames.Implementation_ObjectStore_HashedActionStore();
	default:
	    return null;
	}
    }

    /**
     * @return the <code>int</code> value for this object store.
     * <code>ClassName</code>.
     * @see com.arjuna.ArjunaCommon.Common.ClassName
     */
    
public final static int classNameToType (ClassName c)
    {
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_ActionStore()))
	    return ACTION;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_ActionLogStore()))
	    return ACTIONLOG;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_ShadowingStore()))
	    return SHADOWING;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_FragmentedStore()))
	    return FRAGMENTED;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_VolatileStore()))
	    return VOLATILE;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_HashedStore()))
	    return HASHED;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_ReplicatedStore()))
	    return REPLICATED;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_MappedStore()))
	    return MAPPED;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_SingleTypeMappedStore()))
	    return SINGLETYPEMAPPED;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_ShadowNoFileLockStore()))
	    return SHADOWNOFILELOCK;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_JDBCStore()))
	    return JDBC;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_JDBCActionStore()))
	    return JDBC_ACTION;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_HashedActionStore()))
	    return HASHED_ACTION;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_UserDef0Store()))
	    return USER_DEF_0;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_UserDef1Store()))
	    return USER_DEF_1;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_UserDef2Store()))
	    return USER_DEF_2;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_UserDef3Store()))
	    return USER_DEF_3;
	if (c.equals(ArjunaCoreNames.Implementation_ObjectStore_UserDef4Store()))
	    return USER_DEF_4;

	ErrorStream.warning().println("ObjectStoreType: unknown store: "+c);

	throw new com.arjuna.ArjunaCommon.Common.FatalError("ObjectStoreType: unknown store: "+c);
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

