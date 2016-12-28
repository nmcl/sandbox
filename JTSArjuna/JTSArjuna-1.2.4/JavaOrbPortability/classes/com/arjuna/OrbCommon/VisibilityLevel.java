/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: VisibilityLevel.java,v 1.1 1998/11/12 08:52:37 nmcl Exp $
 */

package com.arjuna.OrbCommon;

public class VisibilityLevel
{
    
public static final int VIS_PRIVATE = 0x0001;
public static final int VIS_PROTECTED = 0x0002;
public static final int VIS_PUBLIC = 0x0004;
public static final int VIS_PACKAGE = 0x0008;
public static final int VIS_ALL = 0xffff;
    
};
