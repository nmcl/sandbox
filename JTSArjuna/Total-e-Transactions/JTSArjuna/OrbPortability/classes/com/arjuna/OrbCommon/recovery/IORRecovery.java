/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: IORRecovery.java,v 1.1.2.1.2.1 2000/12/21 11:21:30 nmcl Exp $
 */

package com.arjuna.OrbCommon.recovery;

import com.arjuna.ArjunaCommon.Common.*;

import java.io.FileNotFoundException;
import java.io.IOException;
import org.omg.CORBA.SystemException;

/**
 * Implementation specific mechanisms for recovering an IOR, i.e.,
 * making an IOR that represents a failed object valid again.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: IORRecovery.java,v 1.1.2.1.2.1 2000/12/21 11:21:30 nmcl Exp $
 * @since JTS 2.0.
 */

public interface IORRecovery
{

public org.omg.CORBA.Object recover (String name, org.omg.CORBA.Object obj,
				     Object[] params) throws SystemException;

};
