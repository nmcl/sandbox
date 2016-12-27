/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AADoctor.java,v 1.3 1998/07/06 10:49:42 nmcl Exp $
 */

package com.arjuna.JavaCosRecovery.AADoctor;

import com.arjuna.JavaCosRecovery.CrashMan.*;
import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaArjunaLite.Atomic.*;
import com.arjuna.JavaArjunaLite.Interface.ObjectStore;
import com.arjuna.JavaArjunaLite.Common.ObjectStoreIterator;
import java.io.*;

import com.arjuna.JavaArjunaLite.Common.ObjectStoreException;
import java.io.IOException;

/*
 * Class: AADoctor
 * Responsible for the co-ordination of atomic action recovery
 */

public class AADoctor
{
    
public AADoctor (PrintStream strm, String recoveringHost,
		 CRRecordList work, ClassName objectStoreImple)
    {
	outStrm = strm;
	host = recoveringHost;
	workList = work;
	objectStoreType = new ClassName(objectStoreImple);
    }

public boolean phase1 ()
    {
	/*
	 * Any unresolved AAs or SAAs that are detected at this phase (just
	 * after a reboot, before the manager is up) are known to be safe for
	 * recovery.
	 */
	
	boolean safe = true;
	boolean anyWork = false;

	outStrm.println("\nAADoctor: Commencing phase 1 recovery scan");

	if (findUnresolvedAAs(safe) > 0)
	    anyWork = true;

	if (findUnresolvedSAAs(safe) > 0)
	    anyWork = true;

	if (findShadowedStates(safe) > 0)
	    anyWork = true;

	outStrm.println("AADoctor: Completed phase 1 recovery scan\n");
	
	return anyWork;
    }

public boolean phase2 ()
    {
	outStrm.println("\nAADoctor: Commencing phase 2 recovery scan");

	boolean safe = false;
	boolean anyWork = false;

	if (findUnresolvedAAs(safe) > 0)
	    anyWork = true;

	if (findUnresolvedSAAs(safe) > 0)
	    anyWork = true;

	outStrm.println("AADoctor: Completed phase 2 recovery scan\n");
	
	return anyWork;
    }

private int findUnresolvedAAs (boolean safe)
    {
	Uid aa_uid = null;
	AtomicAction tempAA = new AtomicAction();
	ObjectStore aaStore = tempAA.store();
	ObjectStoreIterator get_next_aa = new ObjectStoreIterator(aaStore, tempAA.type());
	int numberFound = 0;

	aa_uid = new Uid(Uid.nullUid());
	
	outStrm.println("AADoctor: Searching for local atomic actions...");

	while ( (aa_uid = get_next_aa.iterate()).notEquals(Uid.nullUid()) )
	{
	    AARecord aar = new AARecord(outStrm, safe, aa_uid);

	    if (workList.scan(aar))
	    {
		aar = null;
	    }
	    else
	    {
		outStrm.println("AADoctor: Found AtomicAction with uid = "+aa_uid);
		workList.insert(aar);
		numberFound++;
	    }
	}

	if (numberFound == 0)
	{
	    outStrm.println("AADoctor: No local atomic actions were found");
	}
	else
	{
	    outStrm.println("AADoctor: Found "+numberFound+" local atomic action(s)");
	}

	return numberFound;
    }

private int findUnresolvedSAAs (boolean safe)
    {
	Uid saa_uid = null;

	// no ServerAtomicActions!
	
	/*
	ServerAtomicAction tempSAA = new ServerAtomicAction(Uid.nullUid());;
	ObjectStore saaStore = tempSAA.store();
	ObjectStoreIterator get_next_saa = new ObjectStoreIterator(saaStore, tempSAA.type());
	*/
	
	int numberFound = 0;

	saa_uid = new Uid(Uid.nullUid());
	
	outStrm.println("AADoctor: Searching for server atomic actions...");

	//	while ( (saa_uid = get_next_saa.iterate()).notEquals(Uid.nullUid()) )
	{
	    /*
	    ServerAARecord saar = new ServerAARecord(outStrm, safe, saa_uid, host);

	    if (workList.scan(saar))
	    {
		saar = null;
	    }
	    else
	    {
		outStrm.println("AADoctor: Found ServerAtomicAction with uid = "+saa_uid);
		workList.insert(saar);
		numberFound++;
	    }
	    */
	}

	if (numberFound == 0)
	{
	    outStrm.println("AADoctor: No server atomic actions were found");
	}
	else
	{
	    outStrm.println("AADoctor: Found "+numberFound+" server atomic action(s)");
	}

	return numberFound;
    }

private int findShadowedStates (boolean safe)
    {
	int numberFound = 0;
	InputObjectState typeState = new InputObjectState();
	InputObjectState uidState = new InputObjectState();
	Uid uidFound = null;

	uidFound = new Uid();

	outStrm.println("AADoctor: Searching for uncommitted object states...");

	ObjectStore os = new ObjectStore(objectStoreType);

	try
	{
	    if (os.allTypes(typeState))
	    {
		String p = typeState.unpackString();
	    
		while ((p != null) && (p.length() > 0))
		{
		    if (os.allObjUids(p, uidState, ObjectStore.OS_UNCOMMITTED))
		    {
			uidFound.unpack(uidState);
			
			while (uidFound.notEquals(Uid.nullUid()))
			{
			    ShadowRecord sr = new ShadowRecord(outStrm, safe, uidFound, p, objectStoreType, null);
				
			    if (workList.scan(sr))
			    {
				sr = null;
			    }
			    else
			    {
				outStrm.println("AADoctor: Found ("+p+", "+uidFound+")");
				workList.insert(sr);
				numberFound++;
			    }
			
			    uidFound.unpack(uidState);
			}
		    }

		    p = typeState.unpackString();
		}
	    }
	}
	catch (IOException e)
	{
	    // errors we can't cope with
	}
	catch (ObjectStoreException e)
	{
	}

	os = null;

	if (numberFound == 0)
	{
	    outStrm.println("AADoctor: No uncommitted object states were found");
	}
	else
	{
	    outStrm.println("AADoctor: Found "+numberFound+" uncommitted object state(s)");
	}

	return numberFound;
    }

private PrintStream outStrm;
private String host;
private CRRecordList workList;
private ClassName objectStoreType;

}
