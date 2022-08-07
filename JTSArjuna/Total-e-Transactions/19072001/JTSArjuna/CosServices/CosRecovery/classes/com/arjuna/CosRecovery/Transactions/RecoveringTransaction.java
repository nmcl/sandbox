/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id $
 */
/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.CosRecovery.Transactions;

import com.arjuna.ArjunaCommon.Common.Uid;
import org.omg.CosTransactions.*;
import org.omg.CORBA  .SystemException;
import java.util.Date;

/**
 * interface for cachable recoveredtransactions
 * 
 * Needed because the different types of recovered transaction inherit from
 * (non-recovery) base types by different routes
 *
 * used by @{link TransactionCache}
 *
 * Some methods are present only in the Recovered[*]Transactions.
 * Some are present in all the base types
 */

interface RecoveringTransaction
{
    /** only in Recovered{Server}Transaction */
    public void replayPhase2();
    
    /** only in Recovered{Server}Transaction */
    public int getRecoveryStatus ();

    public void addResourceRecord (Uid rcUid, Resource r);
    
    /** present in both base classes OTS_Transaction and OTS_ServerTransaction */
    public Status get_status () throws SystemException;
    public Status getOriginalStatus ();
    
    public boolean allCompleted();
    /**
     * Tell transaction it is assumed to be complete and should convert itself
     * to the appropriate assumed complete type.
     *
     * @returns true if a change is made, false if already assumed complete
     */
    public boolean assumeComplete();
    
    public void removeOldStoreEntry();
    public String type();
    
    /**
     * When was the transaction last attempted. Only used for assumed complete
     * transactions (so perhaps it ought to be in another interface)
     */
    public Date getLastActiveTime();

}
