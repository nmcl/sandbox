/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: IIOPContext.java,v 1.1 2002/11/25 10:51:42 nmcl Exp $
 */

package com.arjuna.mw.wsas.context.corba;

import com.arjuna.mw.wsas.context.Context;

/**
 */

public interface IIOPContext extends Context
{

    /**
     * @return the position in the Service Context for this information.
     */

    public int position ();
    
}
