/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_Synchronization.java,v 1.1 2000/02/25 14:05:25 nmcl Exp $
 */

package com.arjuna.CosTransactions;

import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.ArjunaCommon.Template.*;
import org.omg.CosTransactions.Synchronization;

public class OTS_Synchronization implements ListElement
{
    
public OTS_Synchronization (Synchronization ptr)
    {
	_ptr = ptr;
	_uid = new Uid();
    }

public void finalize ()
    {
	_ptr = null;
	_uid = null;
    }

public final Synchronization contents ()
    {
	return _ptr;
    }

public final Uid get_uid ()
    {
	return _uid;
    }

private Uid _uid;
private Synchronization _ptr;

};
