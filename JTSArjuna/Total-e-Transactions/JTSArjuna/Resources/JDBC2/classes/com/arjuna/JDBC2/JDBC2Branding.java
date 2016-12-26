/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBC2Branding.java,v 1.1.2.1.16.1.4.1 2000/12/21 11:21:36 nmcl Exp $
 */

package com.arjuna.JDBC2;

/**
 * Connections may need to be branded in an implementation specific
 * manner.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JDBC2Branding.java,v 1.1.2.1.16.1.4.1 2000/12/21 11:21:36 nmcl Exp $
 * @since JTS 2.0.
 */

public interface JDBC2Branding
{

public boolean brand (Object toBrand);
 
}
