/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TxInfo.java,v 1.2.6.1.6.1.2.1.24.1 2000/12/21 11:21:25 nmcl Exp $
 */

package com.arjuna.jta;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosTransactions.*;
import com.arjuna.CosTransactions.tx.*;
import org.omg.CosTransactions.Control;
import javax.transaction.xa.Xid;

class TxInfo
{

    /*
     * IMPORTANT: Do not re-order.
     */

public static final int ASSOCIATED = 0;
public static final int NOT_ASSOCIATED = 1;
public static final int ASSOCIATION_SUSPENDED = 2;
public static final int FAILED = 3;
public static final int UNKNOWN = 4;
    
public TxInfo (Xid xid)
    {
	_xid = xid;
	_thread = Thread.currentThread();

	setState(TxInfo.ASSOCIATED);
    }

public TxInfo (Xid xid, int state)
    {
	_xid = xid;
	_thread = Thread.currentThread();

	setState(state);
    }

public final Thread thread ()
    {
	return _thread;
    }
    
public final Xid xid ()
    {
	return _xid;
    }

public final int getState ()
    {
	return _state;
    }

public final void setState (int s)
    {
	if ((s >= TxInfo.ASSOCIATED) && (s <= TxInfo.UNKNOWN))
	    _state = s;
	else
	    _state = TxInfo.UNKNOWN;
    }

private Xid    _xid;
private int    _state;
private Thread _thread;
    
};
