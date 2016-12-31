/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TxAssociation.java,v 1.3.6.1 2000/05/22 10:23:54 nmcl Exp $
 */

package com.arjuna.CosTransactions.ArjunaOTS;

import org.omg.CosTransactions.Control;
import com.arjuna.CosTransactions.OTS_Control;

/*
 * Instances of these classes are informed whenever a
 * transaction is begun/suspended/resumed/ended.
 * The instance can control whether it wants to be informed
 * about all transactions, or just subtransactions.
 */

public interface TxAssociation
{
 
/*
 * Since one instance could mark the transaction as rollback
 * we could inform subsequent instances. We don't at present.
 */

public void begin (Control tx);
public void begin (OTS_Control tx);

public void commit (Control tx);
public void commit (OTS_Control tx);

public void rollback (Control tx);
public void rollback (OTS_Control tx);

public void suspend (Control tx);
public void suspend (OTS_Control tx);

public void resume (Control tx);
public void resume (OTS_Control tx);

public String name ();
 
/*
 * We could impose some ordering constraints
 * on instances, such that users have the capability
 * of specifying how they are invoked, e.g., A before
 * B. It would affect performance of adding, but if
 * that does not happen frequently it probably doesn't
 * matter.
 */
 
};
