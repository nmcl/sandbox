/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCException.java,v 1.4.2.1 2000/05/19 13:56:36 nmcl Exp $
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
