/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

package java.jts;

public class Status
{

public final static int Active = 0;
public final static int Committed = 1;
public final static int Committing = 2;
public final static int MarkedRollback = 3;
public final static int NoTransaction = 4;
public final static int Prepared = 5;
public final static int Preparing  = 6;
public final static int RolledBack = 7;
public final static int RollingBack = 8;
public final static int Unknown = 9;

};
