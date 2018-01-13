/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TxId.java,v 1.1 2002/11/25 11:00:51 nmcl Exp $
 */

package com.arjuna.mw.wstx.common;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * The transaction identifier. Allows different implementations.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TxId.java,v 1.1 2002/11/25 11:00:51 nmcl Exp $ 
 * @since 1.0.
 */

public interface TxId
{

    public byte[] value ();
    
}
