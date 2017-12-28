/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CoordinatorIdImple.java,v 1.1 2003/01/07 10:33:52 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.model.as.coordinator.jta;

import javax.transaction.xa.Xid;

import com.arjuna.mw.wscf.common.CoordinatorId;

/**
 * This implementation of CoordinatorId wraps the Xid.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: CoordinatorIdImple.java,v 1.1 2003/01/07 10:33:52 nmcl Exp $
 * @since 1.0.
 */

public class CoordinatorIdImple implements CoordinatorId
{

    public CoordinatorIdImple (String name)
    {
	try
	{
	    _value = name.getBytes();
	    _valid = true;
	}
	catch (Exception ex)
	{
	    _valid = false;
	}
    }
    
    public CoordinatorIdImple (Xid xid)
    {
	byte[] tid = xid.getGlobalTransactionId();
	byte[] bqual = xid.getBranchQualifier();
	
	_value = new byte[tid.length+bqual.length];

	int index = 0;
	
	for (int i = 0; i < tid.length; i++)
	{
	    _value[index+i] = tid[i];
	}

	index = tid.length;
	
	for (int j = 0; j < bqual.length; j++)
	{
	    _value[index+j] = bqual[j];
	}

	_valid = true;
    }
    
    public byte[] value ()
    {
	return _value;
    }

    public boolean valid ()
    {
	return _valid;
    }
    
    private byte[]  _value;
    private boolean _valid;
    
}

