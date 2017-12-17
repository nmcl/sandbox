/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Context.java,v 1.2 2003/01/07 10:22:50 nmcl Exp $
 */

package com.arjuna.mw.wsas.context;

/**
 * The context is formed by the various HLSs that are present.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Context.java,v 1.2 2003/01/07 10:22:50 nmcl Exp $
 * @since 1.0.
 */

public interface Context
{
    /**
     * @return the size in bytes.
     */

    public int size ();
    
    /**
     * @return whether or not this is a valid context.
     */

    public boolean valid ();

    /**
     * @return the context in byte format.
     */

    public byte[] contextAsByteArray ();

    /**
     * @return the name of this context.
     */

    public String identifier ();
    
}
