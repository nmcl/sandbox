/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JTA_XAModifier.java,v 1.1.28.1.4.1 2000/12/21 11:21:24 nmcl Exp $
 */

package com.arjuna.jta;

import com.arjuna.jta.JTA_XID;
import javax.transaction.xa.Xid;

/**
 * Instances of this class enable us to work around problems
 * in certain databases (specifically Oracle).
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JTA_XAModifier.java,v 1.1.28.1.4.1 2000/12/21 11:21:24 nmcl Exp $
 * @since JTS 1.2.4.
 */

public interface JTA_XAModifier
{

public Xid createXid (JTA_XID xid) throws NotImplementedException;
 
};
