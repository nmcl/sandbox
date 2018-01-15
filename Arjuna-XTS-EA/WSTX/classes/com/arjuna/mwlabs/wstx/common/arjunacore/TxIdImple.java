/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TxIdImple.java,v 1.1 2002/11/25 11:00:53 nmcl Exp $
 */

package com.arjuna.mwlabs.wstx.common.arjunacore;

import com.hp.mw.ts.arjuna.common.Uid;

import com.arjuna.mw.wstx.common.TxId;

/**
 * This implementation of TxId uses the ArjunaCore Uid class.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TxIdImple.java,v 1.1 2002/11/25 11:00:53 nmcl Exp $
 * @since 1.0.
 */

public class TxIdImple extends Uid implements TxId
{

    public TxIdImple ()
    {
	super();

	_value = stringForm().getBytes();
    }
    
    public TxIdImple (String id)
    {
	super(id);

	_value = stringForm().getBytes();
    }

    public TxIdImple (Uid id)
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

