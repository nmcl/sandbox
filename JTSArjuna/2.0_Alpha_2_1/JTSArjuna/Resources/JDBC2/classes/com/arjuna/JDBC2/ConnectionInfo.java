/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ConnectionInfo.java,v 1.1.2.1 2000/07/17 08:30:13 nmcl Exp $
 */

package com.arjuna.JDBC2;

interface ConnectionInfo
{

public String user ();
public String password ();

public String url ();
public String dynamicClass ();
 
};

