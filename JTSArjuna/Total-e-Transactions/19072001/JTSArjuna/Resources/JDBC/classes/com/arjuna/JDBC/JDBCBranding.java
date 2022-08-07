/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCBranding.java,v 1.1.2.1.2.1 2000/12/21 11:21:32 nmcl Exp $
 */

package com.arjuna.JDBC;

/**
 * Connections may need to be branded in an implementation specific
 * manner.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JDBCBranding.java,v 1.1.2.1.2.1 2000/12/21 11:21:32 nmcl Exp $
 * @since JTS 2.1.
 */

public interface JDBCBranding
{

public boolean brand (Object toBrand);
 
}
