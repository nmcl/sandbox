/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCException.java,v 1.4.2.1.26.1.4.1 2000/12/21 11:21:33 nmcl Exp $
 */

package com.arjuna.JDBC;

/**
 * Exception that may be thrown during transactional JDBC 1.0 processing.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JDBCException.java,v 1.4.2.1.26.1.4.1 2000/12/21 11:21:33 nmcl Exp $
 * @since JTS 1.2.4.
 */

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
