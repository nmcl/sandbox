/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CoordinatorIdImple.java,v 1.1 2003/01/07 10:33:55 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.model.twophase.arjunacore;

import com.hp.mw.ts.arjuna.common.Uid;

import com.arjuna.mw.wscf.common.CoordinatorId;

/**
 * This implementation of CoordinatorId uses the ArjunaCore Uid class.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: CoordinatorIdImple.java,v 1.1 2003/01/07 10:33:55 nmcl Exp $
 * @since 1.0.
 */

public class CoordinatorIdImple extends Uid implements CoordinatorId
{

    public CoordinatorIdImple ()
    {
	super();

	_value = stringForm().getBytes();
    }
    
    public CoordinatorIdImple (String id)
    {
	super(id);

	_value = stringForm().getBytes();
    }

    public CoordinatorIdImple (Uid id)
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

