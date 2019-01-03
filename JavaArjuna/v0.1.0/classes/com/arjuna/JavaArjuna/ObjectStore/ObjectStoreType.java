/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

package com.arjuna.JavaArjuna.ObjectStore;

/*
 * Maintain the types of ObjectStore implementations.
 */

public class ObjectStoreType
{
    public static String SHADOWING_STORE = "ShadowingStore";
    public static String SHADOWNOFILELOCK_STORE = "ShadowNoFileLockStore";
    public static String ACTION_STORE = "ActionStore";

    public static String DEFAULT_STORE = SHADOWNOFILELOCK_STORE;
    public static String DEFAULT_ACTIONSTORE_TYPE = ACTION_STORE;
};
