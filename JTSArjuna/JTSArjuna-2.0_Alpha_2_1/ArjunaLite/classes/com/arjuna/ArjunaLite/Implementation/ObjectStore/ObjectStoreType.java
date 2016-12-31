/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStoreType.java,v 1.1 2000/02/25 14:04:10 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Implementation.ObjectStore;

import com.arjuna.ArjunaLite.ArjunaLiteNames;
import com.arjuna.ArjunaCommon.Common.ClassName;
import java.io.*;

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

public final static ClassName typeToClassName (int rt)
    {
	switch (rt)
	{
	case ACTION:
	    return ArjunaLiteNames.Implementation_ObjectStore_ActionStore();
	case ACTIONLOG:
	    return ArjunaLiteNames.Implementation_ObjectStore_ActionLogStore();
	case SHADOWING:
	    return ArjunaLiteNames.Implementation_ObjectStore_ShadowingStore();
	case FRAGMENTED:
	    return ArjunaLiteNames.Implementation_ObjectStore_FragmentedStore();
	case VOLATILE:
	    return ArjunaLiteNames.Implementation_ObjectStore_VolatileStore();
	case HASHED:
	    return ArjunaLiteNames.Implementation_ObjectStore_HashedStore();
	case REPLICATED:
	    return ArjunaLiteNames.Implementation_ObjectStore_ReplicatedStore();
	case MAPPED:
	    return ArjunaLiteNames.Implementation_ObjectStore_MappedStore();
	case SINGLETYPEMAPPED:
	    return ArjunaLiteNames.Implementation_ObjectStore_SingleTypeMappedStore();
	case USER_DEF_0:
	    return ArjunaLiteNames.Implementation_ObjectStore_UserDef0Store();
	case USER_DEF_1:
	    return ArjunaLiteNames.Implementation_ObjectStore_UserDef1Store();
	case USER_DEF_2:
	    return ArjunaLiteNames.Implementation_ObjectStore_UserDef2Store();
	case USER_DEF_3:
	    return ArjunaLiteNames.Implementation_ObjectStore_UserDef3Store();
	case USER_DEF_4:
	    return ArjunaLiteNames.Implementation_ObjectStore_UserDef4Store();
	case SHADOWNOFILELOCK:
	    return ArjunaLiteNames.Implementation_ObjectStore_ShadowNoFileLockStore();
	default:
	    return null;
	}
    }
    
public final static int classNameToType (ClassName c)
    {
	if (c.equals(ArjunaLiteNames.Implementation_ObjectStore_ActionStore()))
	    return ACTION;
	if (c.equals(ArjunaLiteNames.Implementation_ObjectStore_ActionLogStore()))
	    return ACTIONLOG;
	if (c.equals(ArjunaLiteNames.Implementation_ObjectStore_ShadowingStore()))
	    return SHADOWING;
	if (c.equals(ArjunaLiteNames.Implementation_ObjectStore_FragmentedStore()))
	    return FRAGMENTED;
	if (c.equals(ArjunaLiteNames.Implementation_ObjectStore_VolatileStore()))
	    return VOLATILE;
	if (c.equals(ArjunaLiteNames.Implementation_ObjectStore_HashedStore()))
	    return HASHED;
	if (c.equals(ArjunaLiteNames.Implementation_ObjectStore_ReplicatedStore()))
	    return REPLICATED;
	if (c.equals(ArjunaLiteNames.Implementation_ObjectStore_MappedStore()))
	    return MAPPED;
	if (c.equals(ArjunaLiteNames.Implementation_ObjectStore_SingleTypeMappedStore()))
	    return SINGLETYPEMAPPED;
	if (c.equals(ArjunaLiteNames.Implementation_ObjectStore_ShadowNoFileLockStore()))
	    return SHADOWNOFILELOCK;
	if (c.equals(ArjunaLiteNames.Implementation_ObjectStore_UserDef0Store()))
	    return USER_DEF_0;
	if (c.equals(ArjunaLiteNames.Implementation_ObjectStore_UserDef1Store()))
	    return USER_DEF_1;
	if (c.equals(ArjunaLiteNames.Implementation_ObjectStore_UserDef2Store()))
	    return USER_DEF_2;
	if (c.equals(ArjunaLiteNames.Implementation_ObjectStore_UserDef3Store()))
	    return USER_DEF_3;
	if (c.equals(ArjunaLiteNames.Implementation_ObjectStore_UserDef4Store()))
	    return USER_DEF_4;

	System.err.println("FATAL - ObjectStoreType: unknown store "+c);
	System.exit(0);

	return USER_DEF_4;  // keep compiler happy!
    }

public final static void print (PrintStream strm, int rt)
    {
	ClassName c = typeToClassName(rt);
	
	strm.print(c);

	c = null;
    }

public static final boolean valid (int rt)
    {
	if ((rt < 0) || (rt > SHADOWNOFILELOCK))
	    return false;
	else
	    return true;
    }
    
};
