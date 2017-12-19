/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: IIOPContextImple.java,v 1.2 2003/01/07 10:22:50 nmcl Exp $
 */

package com.arjuna.mwlabs.wsas.context.corba;

import com.arjuna.mw.wsas.context.corba.IIOPContext;

/**
 */

public class IIOPContextImple implements IIOPContext
{

    public int size ()
    {
	return 0;
    }
   
    public boolean valid ()
    {
	return false;
    }
    
    public int position ()
    {
	return 0;
    }

    public byte[] contextAsByteArray ()
    {
	return null;
    }

    public String identifier ()
    {
	return "IIOPContextImple";
    }

    public String toString ()
    {
	return identifier();
    }
    
}
