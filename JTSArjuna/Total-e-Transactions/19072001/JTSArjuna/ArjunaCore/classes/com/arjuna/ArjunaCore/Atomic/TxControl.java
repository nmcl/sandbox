/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TxControl.java,v 1.1.2.1 2001/07/19 13:45:54 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

/*
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: TxControl.java,v 1.1.2.1 2001/07/19 13:45:54 nmcl Exp $
 * @since JTS 2.2.
 */

public class TxControl
{

public static final void enable ()
    {
	InitAction.enable = true;
    }
    
public static final void disable ()
    {
	/*
	 * We could have an implementation that did not return until
	 * all transactions had finished. However, this could take
	 * an arbitrary time, especially if participants could fail.
	 * Since this information is available anyway to the application,
	 * let it handle it.
	 */

	InitAction.enable = false;
    }

public static final boolean isEnabled ()
    {
	return InitAction.enable;
    }

}
