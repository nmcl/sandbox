/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBExceptions.java,v 1.1 2000/02/25 14:09:17 nmcl Exp $
 */

package com.arjuna.OrbCommon;

public class ORBExceptions
{

public static final int BAD_OPERATION_BASE = 100000;
public static final int NOT_FOUND = BAD_OPERATION_BASE+1;
public static final int CANNOT_PROCEED = BAD_OPERATION_BASE+2;
public static final int INVALID_NAME = BAD_OPERATION_BASE+3;

/*
 * We throw this BAD_PARAM when narrow fails.
 */

public static final int BAD_OBJECT_REF = 10029;
    
};
