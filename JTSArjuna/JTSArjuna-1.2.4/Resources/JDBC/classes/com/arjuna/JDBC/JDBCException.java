/*
 * Copyright (C) 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: JDBCException.java,v 1.3 1999/06/22 15:12:35 nmcl Exp $
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
