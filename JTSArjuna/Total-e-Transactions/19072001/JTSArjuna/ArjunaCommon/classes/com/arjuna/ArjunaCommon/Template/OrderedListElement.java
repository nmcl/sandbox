/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrderedListElement.java,v 1.1.28.1 2000/11/02 12:58:46 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

/**
 * Entries in OrderedLists must inherit from this class
 * and define the specified methods.
 */

public interface OrderedListElement
{

    /**
     * Are the two entries equal?
     */

public boolean equals (OrderedListElement e);

/**
 * Is the current entry less than the one in the parameter?
 */

public boolean lessThan (OrderedListElement e);

/**
 * Is the current entry greater than the one in the parameter?
 */
 
public boolean greaterThan (OrderedListElement e);

}

