/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosActivityAdministrationCurrentImple.javatmpl,v 1.1 2001/07/23 13:44:50 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.Activity;

import com.arjuna.Activity.ActivityCoordinator.ActivityCoordinatorWrapper;
import org.omg.CosActivityAdministration.*;
import org.omg.CORBA.*;
import java.util.*;

import java.util.EmptyStackException;
import java.util.NoSuchElementException;
import org.omg.CosActivityAdministration.PropertyGroupNotRegistered;
import org.omg.CosActivityAdministration.PropertyGroupAlreadyRegistered;
import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.SystemException;

public class CosActivityAdministrationCurrentImple extends CosActivityCurrentImple implements  org.omg.CosActivityAdministration .CurrentOperations 
{

public void register_property_group (String property_group_name,
                                     PropertyGroupManager manager,
                                     PropertyGroupAttributes attributes) throws PropertyGroupAlreadyRegistered, SystemException
    {
	throw new BAD_OPERATION("Not implemented!");
    }

public void unregister_property_group (String property_group_name) throws PropertyGroupNotRegistered, SystemException
    {
	throw new BAD_OPERATION("Not implemented!");
    }	
 
};
