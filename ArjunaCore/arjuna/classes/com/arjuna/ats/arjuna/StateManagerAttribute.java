/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: StateManagerAttribute.java,v 1.1 2003/06/19 10:50:13 nmcl Exp $
 */

package com.arjuna.ats.arjuna;

import com.arjuna.ats.arjuna.gandiva.ClassName;

/**
 * Instances of this class may be passed to a StateManager object at
 * construction time and used to configure it.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: StateManagerAttribute.java,v 1.1 2003/06/19 10:50:13 nmcl Exp $
 * @since JTS 1.0.
 */

public class StateManagerAttribute
{

public StateManagerAttribute ()
    {
	objectModel = ObjectModel.SINGLE;
	objectStoreType = null;
    }

public int       objectModel;
public ClassName objectStoreType;
    
}
