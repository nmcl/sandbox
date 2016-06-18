/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ReadOnly.java,v 1.1 2003/02/03 16:24:46 nmcl Exp $
 */

package com.arjuna.wst;

/**
 * ReadOnly: does not need to be informed of the transaction outcome as no
 * state updates were made.
 */

public class ReadOnly implements Vote
{
}
