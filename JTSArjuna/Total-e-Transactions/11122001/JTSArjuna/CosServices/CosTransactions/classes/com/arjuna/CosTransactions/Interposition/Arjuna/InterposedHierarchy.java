/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: InterposedHierarchy.java,v 1.2.8.1 2000/07/31 12:01:44 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition.Arjuna;

import org.omg.CosTransactions.*;
import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.ArjunaCommon.Template.*;
import com.arjuna.CosTransactions.*;
import com.arjuna.CosTransactions.Interposition.OTS_ServerControl;
import com.arjuna.OrbCommon.*;
import java.io.PrintWriter;

public class InterposedHierarchy implements ListElement
{
    
public InterposedHierarchy (OTS_ServerTopLevelAction action)
    {
	_action = action;
    }

public void finalize ()
    {
	_action = null;
    }

public final OTS_ServerTopLevelAction action ()
    {
	return _action;
    }

public Uid get_uid ()
    {
	return ((_action != null) ? _action.get_uid() : Uid.nullUid());
    }
    
public final void print (PrintWriter strm)
    {
	strm.println("InterposedHierarchy:");

	if (_action != null)
	{
	    strm.print(_action.get_uid());

	    HashList children = _action.getChildren();

	    if (children != null)
	    {
		HashListIterator iter = new HashListIterator(children);
		OTS_ServerNestedAction childPtr = (OTS_ServerNestedAction) iter.iterate();

		while (childPtr != null)
		{
		    strm.print("\n"+childPtr.get_uid());

		    childPtr.printChildren(strm, 2);
		    childPtr = (OTS_ServerNestedAction) iter.iterate();
		}

		iter = null;
	    }
	}
	else
	    strm.print("EMPTY");
    }

private OTS_ServerTopLevelAction _action;

};
