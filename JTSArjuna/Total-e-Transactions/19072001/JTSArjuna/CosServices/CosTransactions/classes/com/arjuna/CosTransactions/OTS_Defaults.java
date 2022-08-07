/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK
 *
 * $Id: OTS_Defaults.java,v 1.1.30.1.2.1.2.1 2000/12/21 11:21:15 nmcl Exp $
 */

package com.arjuna.CosTransactions;

/**
 * Some default values for the OTS.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_Defaults.java,v 1.1.30.1.2.1.2.1 2000/12/21 11:21:15 nmcl Exp $
 * @since JTS 1.0.
 */

public class OTS_Defaults
{

    /**
     * Does the receiving transactional object always need to be invoked within
     * the context of a transaction?
     */

public static boolean needTransactionContext = false;

    /**
     * Do we always propagate the transaction hierarchy regardless of
     * whether or not the receiving object is derived from
     * CosTransactions::TransactionalObject?
     */

public static boolean alwaysPropagateContext = false;

}
