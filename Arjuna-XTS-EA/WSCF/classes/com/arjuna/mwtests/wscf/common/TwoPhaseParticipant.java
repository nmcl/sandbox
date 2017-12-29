/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TwoPhaseParticipant.java,v 1.1 2003/01/07 10:37:17 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.common;

import com.arjuna.mw.wscf.model.twophase.common.*;
import com.arjuna.mw.wscf.model.twophase.outcomes.*;
import com.arjuna.mw.wscf.model.twophase.participants.*;
import com.arjuna.mw.wscf.model.twophase.exceptions.*;
import com.arjuna.mw.wscf.model.twophase.vote.*;

import com.arjuna.mw.wscf.exceptions.*;

import com.arjuna.mw.wsas.exceptions.SystemException;
import com.arjuna.mw.wsas.exceptions.WrongStateException;
import com.arjuna.mw.wsas.exceptions.ProtocolViolationException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TwoPhaseParticipant.java,v 1.1 2003/01/07 10:37:17 nmcl Exp $
 * @since 1.0.
 */

public class TwoPhaseParticipant implements Participant
{

    public TwoPhaseParticipant ()
    {
    }
    
    public Vote prepare () throws InvalidParticipantException, WrongStateException, HeuristicHazardException, HeuristicMixedException, SystemException
    {
	System.out.println("TwoPhaseParticipant.prepare");
	
	return new VoteConfirm();
    }

    public void confirm () throws InvalidParticipantException, WrongStateException, HeuristicHazardException, HeuristicMixedException, HeuristicCancelException, SystemException
    {
	System.out.println("TwoPhaseParticipant.confirm");
    }
    
    public void cancel () throws InvalidParticipantException, WrongStateException, HeuristicHazardException, HeuristicMixedException, HeuristicConfirmException, SystemException
    {
	System.out.println("TwoPhaseParticipant.cancel");
    }
    
    public void confirmOnePhase () throws InvalidParticipantException, WrongStateException, HeuristicHazardException, HeuristicMixedException, HeuristicCancelException, SystemException
    {
	System.out.println("TwoPhaseParticipant.confirmOnePhase");
    }
    
    public void forget () throws InvalidParticipantException, WrongStateException, SystemException
    {
	System.out.println("TwoPhaseParticipant.forget");
    }

}
