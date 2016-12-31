/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: StateManagerAttribute.java,v 1.1 2000/02/25 14:03:57 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import com.arjuna.ArjunaLite.Atomic.ObjectModel;
import com.arjuna.ArjunaCommon.Common.ClassName;

public class StateManagerAttribute
{

public StateManagerAttribute ()
    {
	remoteState = false;
	useStoreLocation = false;
	storeLocations = null;
	objectModel = ObjectModel.SINGLE;
	objectStoreType = null;
    }

public boolean remoteState;
public boolean useStoreLocation;
public String[] storeLocations;
public int objectModel;
public ClassName objectStoreType;
    
};
