/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: GlobalIdImple.java,v 1.1 2002/11/25 10:51:46 nmcl Exp $
 */

package com.arjuna.mwlabs.wsas.common.arjunacore;

import com.hp.mw.ts.arjuna.common.Uid;

import com.arjuna.mw.wsas.common.GlobalId;

/**
 * This implementation of GlobalId uses the ArjunaCore Uid class.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: GlobalIdImple.java,v 1.1 2002/11/25 10:51:46 nmcl Exp $
 * @since 1.0.
 */

public class GlobalIdImple extends Uid implements GlobalId
{

    public GlobalIdImple ()
    {
	super();

	_value = stringForm().getBytes();
    }
    
    public GlobalIdImple (String id)
    {
	super(id);

	_value = stringForm().getBytes();
    }
    
    public byte[] value ()
    {
	return _value;
    }
    
    private byte[] _value;
    
}

