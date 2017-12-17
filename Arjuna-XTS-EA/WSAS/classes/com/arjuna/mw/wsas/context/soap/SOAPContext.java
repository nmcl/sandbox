/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SOAPContext.java,v 1.3 2003/02/13 11:26:10 nmcl Exp $
 */

package com.arjuna.mw.wsas.context.soap;

import com.arjuna.mw.wsas.context.Context;

/**
 */

public interface SOAPContext extends Context
{

    /**
     * @return the context element object.
     */

    public org.w3c.dom.Element context ();

    /**
     * @return <code>true</code> if the context data goes in the header,
     * <code>false</code> otherwise.
     */

    public boolean header ();

    /**
     * Initialise the implementation using the parameter provided.
     */

    public void initialise (Object obj) throws java.security.InvalidParameterException;
    
}
