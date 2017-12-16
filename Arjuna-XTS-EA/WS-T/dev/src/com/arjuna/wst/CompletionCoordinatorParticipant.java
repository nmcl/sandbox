/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CompletionCoordinatorParticipant.java,v 1.1 2003/02/19 17:28:58 nmcl Exp $
 */

package com.arjuna.wst;

/**
 */

public interface CompletionCoordinatorParticipant
{

    public void commit () throws TransactionRolledBackException, UnknownTransactionException, SystemException;
    
    public void rollback () throws UnknownTransactionException, SystemException;
    
}
