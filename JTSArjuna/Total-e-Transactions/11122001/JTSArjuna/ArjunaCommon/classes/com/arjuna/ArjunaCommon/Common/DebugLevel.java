/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DebugLevel.java,v 1.2.8.1.22.1.4.2.2.1.32.1.22.1 2001/10/31 12:07:03 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

/**
 * The various levels of debugging that are available.
 * <ul>
 * <li>NO_DEBUGGING	   = 0x0 (no debugging).
 * <li>CONSTRUCTORS	   = 0x0001 (only output from constructors).
 * <li>DESTRUCTORS	    = 0x0002 (only output from finalizers).
 * <li>CONSTRUCT_AND_DESTRUCT = CONSTRUCTORS | DESTRUCTORS.
 * <li>FUNCTIONS	      = 0x0010 (only output from methods).
 * <li>OPERATORS	      = 0x0020 (only output from methods such as <code>equals, notEquals</code>).
 * <li>FUNCS_AND_OPS	  = FUNCTIONS | OPERATORS.
 * <li>ALL_NON_TRIVIAL	= CONSTRUCT_AND_DESTRUCT | FUNCTIONS | OPERATORS.
 * <li>TRIVIAL_FUNCS	  = 0x0100 (only output from trivial methods).
 * <li>TRIVIAL_OPERATORS      = 0x0200 (only output from trivial operators).
 * <li>ALL_TRIVIAL	    = TRIVIAL_FUNCS | TRIVIAL_OPERATORS.
 * <li>ERROR_MESSAGES	 = 0x0400 (only output from debugging error/warning messages).
 * <li>FULL_DEBUGGING	 = 0xffff (output all debugging messages).
 * </ul>
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: DebugLevel.java,v 1.2.8.1.22.1.4.2.2.1.32.1.22.1 2001/10/31 12:07:03 nmcl Exp $
 * @since JTS 1.2.4.
 */

public class DebugLevel
{

public static final long NO_DEBUGGING = 0;
public static final long CONSTRUCTORS = 0x00000001;
public static final long DESTRUCTORS = 0x00000002;
public static final long CONSTRUCT_AND_DESTRUCT = CONSTRUCTORS | DESTRUCTORS;
public static final long FUNCTIONS = 0x00000010;
public static final long OPERATORS = 0x00000020;
public static final long FUNCS_AND_OPS = FUNCTIONS | OPERATORS;
public static final long ALL_NON_TRIVIAL = CONSTRUCT_AND_DESTRUCT | FUNCTIONS | OPERATORS;
public static final long TRIVIAL_FUNCS = 0x00000100;
public static final long TRIVIAL_OPERATORS = 0x00000200;
public static final long ALL_TRIVIAL = TRIVIAL_FUNCS | TRIVIAL_OPERATORS;
public static final long ERROR_MESSAGES = 0x00000400;
public static final long FULL_DEBUGGING = 0xffffffff;

    /**
     * @since JTS 2.1.2.
     */

public static final long getLevel (String level)
    {
	if (level.equals("NO_DEBUGGING"))
	    return NO_DEBUGGING;
	if (level.equals("CONSTRUCTORS"))
	    return CONSTRUCTORS;
	if (level.equals("DESTRUCTORS"))
	    return DESTRUCTORS;
	if (level.equals("CONSTRUCT_AND_DESTRUCT"))
	    return CONSTRUCT_AND_DESTRUCT;
	if (level.equals("FUNCTIONS"))
	    return FUNCTIONS;
	if (level.equals("OPERATORS"))
	    return OPERATORS;
	if (level.equals("FUNCS_AND_OPS"))
	    return FUNCS_AND_OPS;
	if (level.equals("ALL_NON_TRIVIAL"))
	    return ALL_NON_TRIVIAL;
	if (level.equals("TRIVIAL_FUNCS"))
	    return TRIVIAL_FUNCS;
	if (level.equals("TRIVIAL_OPERATORS"))
	    return TRIVIAL_OPERATORS;
	if (level.equals("ALL_TRIVIAL"))
	    return ALL_TRIVIAL;
	if (level.equals("ERROR_MESSAGES"))
	    return ERROR_MESSAGES;
	if (level.equals("FULL_DEBUGGING"))
	    return FULL_DEBUGGING;
	
	return NO_DEBUGGING;
    }
    
    /**
     * @since JTS 2.1.2.
     */

public static final String printString (long level)
    {
	if (level == NO_DEBUGGING)
	    return "NO_DEBUGGING";
	if (level == CONSTRUCTORS)
	    return "CONSTRUCTORS";
	if (level == DESTRUCTORS)
	    return "DESTRUCTORS";
	if (level == CONSTRUCT_AND_DESTRUCT)
	    return "CONSTRUCT_AND_DESTRUCT";
	if (level == FUNCTIONS)
	    return "FUNCTIONS";
	if (level == OPERATORS)
	    return "OPERATORS";
	if (level == FUNCS_AND_OPS)
	    return "FUNCS_AND_OPS";
	if (level == ALL_NON_TRIVIAL)
	    return "ALL_NON_TRIVIAL";
	if (level == TRIVIAL_FUNCS)
	    return "TRIVIAL_FUNCS";
	if (level == TRIVIAL_OPERATORS)
	    return "TRIVIAL_OPERATORS";
	if (level == ALL_TRIVIAL)
	    return "ALL_TRIVIAL";
	if (level == ERROR_MESSAGES)
	    return "ERROR_MESSAGES";
	if (level == FULL_DEBUGGING)
	    return "FULL_DEBUGGING";

	return "NO_DEBUGGING";
    }

}

