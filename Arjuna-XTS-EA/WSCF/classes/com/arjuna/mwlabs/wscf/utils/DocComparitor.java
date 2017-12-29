/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DocComparitor.java,v 1.2 2003/01/17 15:39:34 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.utils;

import com.arjuna.mw.wscf.common.CoordinatorXSD;

import com.arjuna.mw.wscf.utils.CoordinationXML;

import org.w3c.dom.*;

import java.util.List;

import java.security.InvalidParameterException;

/**
 * The DocComparitor takes two XML documents describing a coordinator's
 * protocol and tests for equality. Two documents are equal if the have
 * exactly the same elements and attributes, no matter what the order of
 * occurrence.
 *
 * The comapritor may be extended to allow subset or superset comparisons.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: DocComparitor.java,v 1.2 2003/01/17 15:39:34 nmcl Exp $
 * @since 1.0.
 */

public class DocComparitor
{

    public DocComparitor ()
    {
    }

    /**
     * Are the two documents equal? We assume that the protocol names are
     * unique, so can check for equality simply on that. If we want to check
     * individual elements and attributes then we could.
     */

    public boolean equals (org.w3c.dom.Document doc1, org.w3c.dom.Document doc2) throws InvalidParameterException
    {
	if (doc1 == null)
	    throw new InvalidParameterException("First parameter is null!");
	
	if (doc2 == null)
	    throw new InvalidParameterException("Second parameter is null!");

	CoordinationXML protocol1 = new CoordinationXML(doc1);
	CoordinationXML protocol2 = new CoordinationXML(doc2);
	
	return protocol1.equals(protocol2);
    }

}
