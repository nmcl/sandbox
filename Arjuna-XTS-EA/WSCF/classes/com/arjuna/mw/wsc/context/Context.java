/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Context.java,v 1.1 2003/02/26 12:05:43 nmcl Exp $
 */

package com.arjuna.mw.wsc.context;

import com.arjuna.wsc.messaging.CoordinationContext;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: Context.java,v 1.1 2003/02/26 12:05:43 nmcl Exp $
 * @since 1.0.
 */

public interface Context
{

    public CoordinationContext getCoordinationContext ();
    
    public void setCoordinationContext (CoordinationContext cc);

    public boolean equals (Object obj);
    
}
