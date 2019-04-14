/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Hewlett Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: SynchronizationRecord.java,v 1.1 2003/06/19 10:50:17 nmcl Exp $
 */

package com.arjuna.ats.arjuna.coordinator;

import com.arjuna.ats.arjuna.StateManager;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.state.*;
import com.arjuna.ats.arjuna.gandiva.ClassName;
import com.arjuna.ats.arjuna.gandiva.inventory.Inventory;

import com.arjuna.ats.internal.arjuna.template.ListElement;

import java.io.PrintWriter;

import java.io.IOException;

/*
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: SynchronizationRecord.java,v 1.1 2003/06/19 10:50:17 nmcl Exp $
 * @since 3.0.
 */

public interface SynchronizationRecord extends ListElement
{

    public boolean beforeCompletion ();

    public boolean afterCompletion (int status);

}

