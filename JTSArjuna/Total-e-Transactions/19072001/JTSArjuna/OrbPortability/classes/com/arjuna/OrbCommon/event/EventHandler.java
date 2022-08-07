/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: EventHandler.java,v 1.1.2.1.2.2 2001/01/11 14:17:03 nmcl Exp $
 */

package com.arjuna.OrbCommon.event;

import org.omg.CORBA.Object;

/*
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: EventHandler.java,v 1.1.2.1.2.2 2001/01/11 14:17:03 nmcl Exp $
 * @since JTS 2.1.
 */

public interface EventHandler
{

public void connected (org.omg.CORBA.Object obj);
public void disconnected (org.omg.CORBA.Object obj);

public String name ();
 
}
