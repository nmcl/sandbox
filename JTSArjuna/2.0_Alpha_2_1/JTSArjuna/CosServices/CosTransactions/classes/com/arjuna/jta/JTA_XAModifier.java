/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JTA_XAModifier.java,v 1.1 2000/04/06 11:00:30 nmcl Exp $
 */

package com.arjuna.jta;

import com.arjuna.jta.JTA_XID;
import javax.transaction.xa.Xid;

/*
 * Instances of this class enable us to work around problems
 * in certain databases.
 */

public interface JTA_XAModifier
{

public Xid createXid (JTA_XID xid) throws NotImplementedException;
 
};
