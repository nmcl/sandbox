/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Brander.java,v 1.1 2003/06/19 10:50:31 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.jdbc.brandings;

/**
 * Connections may need to be branded in an implementation specific
 * manner.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Brander.java,v 1.1 2003/06/19 10:50:31 nmcl Exp $
 * @since JTS 2.0.
 */

public interface Brander
{

    public boolean brand (Object toBrand);
 
}
