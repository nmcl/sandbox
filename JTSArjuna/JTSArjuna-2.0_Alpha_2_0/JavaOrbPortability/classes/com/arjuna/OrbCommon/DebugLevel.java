/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DebugLevel.java,v 1.1 1998/11/12 08:52:33 nmcl Exp $
 */

package com.arjuna.OrbCommon;

public class DebugLevel
{

public static final int NO_DEBUGGING = 0;
public static final int CONSTRUCTORS = 0x0001;
public static final int DESTRUCTORS = 0x0002;
public static final int CONSTRUCT_AND_DESTRUCT = CONSTRUCTORS | DESTRUCTORS;
public static final int FUNCTIONS = 0x0010;
public static final int OPERATORS = 0x0020;
public static final int FUNCS_AND_OPS = FUNCTIONS | OPERATORS;
public static final int ALL_NON_TRIVIAL = CONSTRUCT_AND_DESTRUCT | FUNCTIONS | OPERATORS;
public static final int TRIVIAL_FUNCS = 0x0100;
public static final int TRIVIAL_OPERATORS = 0x0200;
public static final int ALL_TRIVIAL = TRIVIAL_FUNCS | TRIVIAL_OPERATORS;
public static final int ERROR_MESSAGES = 0x0400;
public static final int FULL_DEBUGGING = 0xffff;

};
