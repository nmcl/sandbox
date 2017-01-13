/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_Synchronization.java,v 1.2 1998/07/06 13:28:43 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions;

import com.arjuna.JavaGandiva.Common.Uid;
import com.arjuna.JavaGandiva.Template.*;
import org.omg.CosTransactions.Synchronization;

public class OTS_Synchronization implements UidListElement
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
