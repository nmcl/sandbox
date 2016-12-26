/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBExceptions.java,v 1.1.28.1.4.1 2000/12/21 11:21:28 nmcl Exp $
 */

package com.arjuna.OrbCommon;

/**
 * Some exception values we may set.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ORBExceptions.java,v 1.1.28.1.4.1 2000/12/21 11:21:28 nmcl Exp $
 * @since JTS 1.0.
 */

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
    
}
