/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TwoPhaseParticipant.java,v 1.1 2003/02/03 11:01:27 nmcl Exp $
 */

package com.arjuna.mw.wst.resources;

import com.arjuna.mw.wst.vote.Vote;

import com.arjuna.mw.wst.exceptions.*;

/**
 * The TwoPhaseParticipant. As with all traditional TP implementations, the
 * one-phase commit optimisation is supported.
 */

public interface TwoPhaseParticipant
{

    public Vote prepare () throws WrongStateException, HeuristicHazardException, HeuristicMixedException, UnknownTransactionException, SystemException;

    public void commit () throws WrongStateException, HeuristicHazardException, HeuristicMixedException, HeuristicRollbackException, UnknownTransactionException, SystemException;

    public void rollback () throws WrongStateException, HeuristicHazardException, HeuristicMixedException, HeuristicCommitException, UnknownTransactionException, SystemException;

    public void commitOnePhase () throws WrongStateException, HeuristicHazardException, HeuristicMixedException, HeuristicRollbackException, UnknownTransactionException, SystemException;
    
}
