/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: BasicListIterator.java,v 1.1 2003/06/19 10:50:38 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.template;

/**
 * An iterator for BasicLists.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: BasicListIterator.java,v 1.1 2003/06/19 10:50:38 nmcl Exp $
 * @since JTS 1.0.
 */

public class BasicListIterator extends SimpleListIterator
{

    /**
     * Create a new iterator which will be used to iterate over
     * the provided list.
     */

public BasicListIterator (BasicList list)
    {
	super(list);
    }

}
