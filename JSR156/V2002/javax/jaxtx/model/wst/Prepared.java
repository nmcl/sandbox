/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Prepared.java,v 1.1 2003/02/03 16:24:45 nmcl Exp $
 */

package com.arjuna.wst;

/**
 * Prepared: it is prepared to commit or rollback depending on the final
 * transaction outcome, and it has made sufficient state updates persistent
 * to accomplish this.
 */

public class Prepared implements Vote
{
}
