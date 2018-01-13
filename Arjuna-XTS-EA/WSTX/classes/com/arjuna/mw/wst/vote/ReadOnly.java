/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ReadOnly.java,v 1.1 2003/02/03 11:01:27 nmcl Exp $
 */

package com.arjuna.mw.wst.vote;

/**
 * ReadOnly: does not need to be informed of the transaction outcome as no
 * state updates were made.
 */

public class ReadOnly implements Vote
{
}
