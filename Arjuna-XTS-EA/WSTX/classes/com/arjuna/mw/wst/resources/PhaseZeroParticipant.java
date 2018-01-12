/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PhaseZeroParticipant.java,v 1.1 2003/02/03 11:01:27 nmcl Exp $
 */

package com.arjuna.mw.wst.resources;

import com.arjuna.mw.wst.exceptions.*;

/**
 * The PhaseZeroParticipant. This is half of a traditional Synchronization
 * interface.
 */

public interface PhaseZeroParticipant
{

    public void phaseZero () throws UnknownTransactionException, SystemException;
    
}
