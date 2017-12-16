/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CompletionWithAckCoordinatorParticipant.java,v 1.2 2003/02/24 10:05:19 nmcl Exp $
 */

package com.arjuna.wst;

/**
 */

public interface CompletionWithAckCoordinatorParticipant extends CompletionCoordinatorParticipant
{

    public void notified () throws UnknownTransactionException, SystemException;

}
