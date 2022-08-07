/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ConnectionInfo.java,v 1.1.2.1.10.1.10.1.12.1 2001/02/22 11:19:17 nmcl Exp $
 */

package com.arjuna.JDBC2;

interface ConnectionInfo
{

public String user ();
public String password ();

public String url ();
public String dynamicClass ();

public String dataSourceName ();
 
}
