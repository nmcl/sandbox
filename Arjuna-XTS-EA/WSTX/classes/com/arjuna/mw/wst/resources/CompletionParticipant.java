/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CompletionParticipant.java,v 1.1 2003/02/03 11:01:26 nmcl Exp $
 */

package com.arjuna.mw.wst.resources;

import com.arjuna.mw.wst.exceptions.*;

/**
 * The CompletionParticipant.
 */

public interface CompletionParticipant
{

    public void committed () throws UnknownTransactionException, SystemException;
    public void aborted () throws UnknownTransactionException, SystemException;
    
}
