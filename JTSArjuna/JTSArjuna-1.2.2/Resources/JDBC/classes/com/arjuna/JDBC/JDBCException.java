/*
 * Copyright (C) 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: JDBCException.java,v 1.1.2.1 1999/02/12 09:47:03 nmcl Exp $
 */

package com.arjuna.JDBC;

public class JDBCException extends Exception
{

public JDBCException ()
    {
    }

public JDBCException (String s)
    {
	super(s);
    }
    
};
