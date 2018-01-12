/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CompletionWithAckParticipant.java,v 1.1 2003/02/03 11:01:26 nmcl Exp $
 */

package com.arjuna.mw.wst.resources;

/**
 * The CompletionWithAckParticipant. The signature is essentially the same
 * as the CompletionParticipant, but we may want to be able to differentiate
 * between the two types internally. Hence the two distinct interfaces.
 */

public interface CompletionWithAckParticipant extends CompletionParticipant
{
}
