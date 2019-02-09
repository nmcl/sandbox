/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StateManagerAttribute.java,v 1.3 1998/07/06 13:26:33 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

import com.arjuna.JavaArjuna.ClassLib.ObjectModel;

public class StateManagerAttribute
{

public StateManagerAttribute ()
    {
	remoteState = false;
	useStoreLocation = false;
	storeLocations = null;
	objectModel = ObjectModel.SINGLE;
    }

public boolean remoteState;
public boolean useStoreLocation;
public String[] storeLocations;
public int objectModel;
    
};
