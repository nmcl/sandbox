/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TwoPCParticipant.java,v 1.3 2003/03/24 11:01:42 nmcl Exp $
 */

package com.arjuna.wst;

/**
 * The TwoPhaseParticipant. As with all traditional TP implementations, the
 * one-phase commit optimisation is supported.
 */

public interface TwoPCParticipant
{

    /**
     * Perform any work necessary to allow it to either commit or rollback
     * the work performed by the Web service under the scope of the
     * transaction. The implementation is free to do whatever it needs to in
     * order to fulfill the implicit contract between it and the coordinator.
     *
     * @return an indication of whether it can prepare or not.
     * @see com.arjuna.wst.Vote
     */

    public Vote prepare () throws WrongStateException, SystemException;

    /**
     * The participant should make permanent the work that it controls.
     */

    public void commit () throws WrongStateException, SystemException;

    /**
     * The participant should undo the work that it controls. The participant
     * will then return an indication of whether or not it succeeded.
     */

    public void rollback () throws WrongStateException, SystemException;

    /**
     * There is only a single TwoPCParticipant registered with the transaction.
     * The coordinator can optimize the protocol and simply tell the
     * participant to commit: there is no need for a preparatory phase since
     * consensus is implicit.
     */

    public void commitOnePhase () throws TransactionRolledBackException, WrongStateException, SystemException;

    /**
     * During recovery the participant can enquire as to the status of the
     * transaction it was registered with. If that transaction is no longer
     * available (has rolled back) then this operation will be invoked by the
     * coordination service.
     */

    public void unknown () throws SystemException;

    /**
     * During recovery the participant can enquire as to the status of the
     * transaction it was registered with. If an error occurs (e.g., the
     * transaction service is unavailable) then this operation will be invoked.
     */
    
    void error () throws SystemException;
    
}
