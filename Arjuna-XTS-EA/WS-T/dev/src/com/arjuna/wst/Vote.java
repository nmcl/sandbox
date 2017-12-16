/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Vote.java,v 1.2 2003/03/24 11:01:42 nmcl Exp $
 */

package com.arjuna.wst;

/**
 * When asked to prepare, a 2PC participant returns one of three types of
 * vote:
 *
 * ReadOnly: does not need to be informed of the transaction outcome as no
 * state updates were made.
 * Prepared: it is prepared to commit or rollback depending on the final
 * transaction outcome, and it has made sufficient state updates persistent
 * to accomplish this.
 * Aborted: the participant has aborted and the transaction should also
 * attempt to do so.
 *
 * @see com.arjuna.wst.ReadOnly
 * @see com.arjuna.wst.Prepared
 * @see com.arjuna.wst.Aborted
 */

public interface Vote
{
}
