/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: FdCache.java,v 1.1 2003/06/19 10:50:32 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.objectstore;

import com.arjuna.ats.arjuna.common.Uid;
import java.io.*;

class CacheEntry
{
    
public void CacheEntry (File f)
    {
	_theFile = f;
    }
    
public File file ()
    {
	return _theFile;
    }
    
CacheEntry _next;
CacheEntry _prev;

private File _theFile;
    
}

class FdCache
{

public FdCache ()
    {
	this(FdCache.cacheSize);
    }

public FdCache (int size)
    {
	_head = _tail = null;
    }

/* XXX: Not yet finished
public synchronized File scanCache (Uid u)
    {
    }
    
public synchronized File enterIntoCache (Uid u)
    {
    }
    
public void purgeFromCache (Uid u)
    {
    }
    
private boolean compactCache ()
    {
    }
*/
    
private CacheEntry _head;
private CacheEntry _tail;
    
private static final int cacheSize = 256;

}
