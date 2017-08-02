/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClassStore.java,v 1.2 1997/06/08 19:18:28 nmcl Exp $
 */

package RMISystem.net;

class ClassStore
{

public ClassStore (byte[] theClass)
    {
	_theClass = theClass;
    }

public byte[] classBytes ()
    {
	return _theClass;
    }
    
private byte[] _theClass;
    
};
