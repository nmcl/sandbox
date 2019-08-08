/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: VisibilityLevel.java,v 1.2.4.1.6.1.22.1.4.2.2.1.32.1.22.1 2001/10/31 12:07:04 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

/**
 * The various visibility levels that are available.
 * One per type of method/variable scope.
 *
 * <ul>
 * <li>VIS_PRIVATE = 0x0001 (only from private methods).
 * <li>VIS_PROTECTED = 0x0002 (only from protected methods).
 * <li>VIS_PUBLIC = 0x0004 (only from public methods).
 * <li>VIS_PACKAGE = 0x0008 (only from package methods).
 * <li>VIS_ALL = 0xffff (from all methods).
 * </ul>
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: VisibilityLevel.java,v 1.2.4.1.6.1.22.1.4.2.2.1.32.1.22.1 2001/10/31 12:07:04 nmcl Exp $
 * @since JTS 1.2.4.
 */

public class VisibilityLevel
{

public static final long VIS_PRIVATE = 0x00000001;
public static final long VIS_PROTECTED = 0x00000002;
public static final long VIS_PUBLIC = 0x00000004;
public static final long VIS_PACKAGE = 0x00000008;
public static final long VIS_ALL = 0xffffffff;

    /**
     * @since JTS 2.1.2.
     */

public static final long getLevel (String level)
    {
	if (level.equals("VIS_PRIVATE"))
	    return VIS_PRIVATE;
	if (level.equals("VIS_PROTECTED"))
	    return VIS_PROTECTED;
	if (level.equals("VIS_PUBLIC"))
	    return VIS_PUBLIC;
	if (level.equals("VIS_PACKAGE"))
	    return VIS_PACKAGE;
	if (level.equals("VIS_ALL"))
	    return VIS_ALL;
	
	return VIS_ALL;
    }
    
    /**
     * @since JTS 2.1.2.
     */

public static final String printString (long level)
    {
	if (level == VIS_PRIVATE)
	    return "VIS_PRIVATE";
	if (level == VIS_PROTECTED)
	    return "VIS_PROTECTED";
	if (level == VIS_PUBLIC)
	    return "VIS_PUBLIC";
	if (level == VIS_PACKAGE)
	    return "VIS_PACKAGE";
	if (level == VIS_ALL)
	    return "VIS_ALL";

	return "VIS_ALL";
    }

};
