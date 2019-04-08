/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DefaultActionManager.java,v 1.1 2001/07/23 13:44:52 nmcl Exp $
 */

package com.arjuna.Activity.Actions;

import org.omg.CosActivity.*;
import org.omg.CORBA.*;
import java.io.PrintWriter;

import org.omg.CosActivity.ActionError;
import org.omg.CosActivity.AlreadyDestroyed;
import org.omg.CORBA.SystemException;

public class DefaultActionManager
{

public Outcome process_signal (Action act, Signal sig) throws ActionError, SystemException
    {
	act.process_signal(sig);
    }

public void destroy (Action act) throws AlreadyDestroyed, SystemException
    {
	act.destroy();
    }

};
