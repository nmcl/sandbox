/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CRRecordList.java,v 1.2 1998/07/06 10:49:49 nmcl Exp $
 */

package com.arjuna.JavaCosRecovery.CrashMan;

import java.io.PrintStream;

public class CRRecordList
{
    
public CRRecordList (PrintStream strm)
    {
	outStrm = strm;
	listHead = null;
	listTail = null;
	noEntries = 0;
    }

public void finalize ()
    {
	while(listHead != null)
	    remove(listHead);
    }

public boolean scan (CrashRecRecord newRecord)
    {
	boolean found = false;
	
	if (noEntries != 0)
	{
	    CrashRecRecord temp = listHead;
	    
	    while (temp != null)
	    {
		if ( temp.equals(newRecord))
		    found = true;
		
		temp = temp.next;
	    }
	}
	
	return found;
    }

public boolean insert (CrashRecRecord newRecord)
    {
	scan(newRecord);

	if (noEntries == 0)
	{
	    // Insert as only element
	    listHead = listTail = newRecord;
	    noEntries = 1;
	}
	else
	{
	    boolean inserted = false;
	    CrashRecRecord temp = listHead;

	    while (!inserted)
	    {
		if (newRecord.type() < temp.type())
		{
		    newRecord.next = temp;
		    newRecord.previous = temp.previous;

		    if (temp.previous != null)
		    {
			temp.previous.next = newRecord;
		    }
		    else
		    {
			// Insert as listHead element
			listHead = newRecord;
		    }
		    
		    temp.previous = newRecord;
		    inserted = true;
		}
		else
		{
		    if (temp.next != null)
			temp = temp.next;
		    else
		    {
			//Insert as listTail element
			temp.next = newRecord;
			newRecord.previous = temp;
			listTail = newRecord;
			inserted = true;
		    }
		}
	    }
	    
	    noEntries++;
	}
	
	return true;
    }

public int getNoEntries ()
    {
	return noEntries;
    }

public int getNoEntries (int t)
    {
	int count = 0;

	if (noEntries != 0)
	{
	    CrashRecRecord temp = listHead;
	    
	    while (temp != null)
	    {
		if (temp.type() == t)
		    count++;

		temp = temp.next;
	    }
	}

	return count;
    }

public int getWorkLevel ()
    {
	int count = 0;
	
	if (noEntries != 0)
	{
	    CrashRecRecord temp = listHead;
	    
	    while (temp != null)
	    {
		if (temp.getRecoveryStatus() != RecoveryStatus.COMPLETE)
		    count++;
		
		temp = temp.next;
	    }
	}
	
	return count;
    }

public int getWorkLevel (int t)
    {
	int count = 0;
	
	if (noEntries != 0)
	{
	    CrashRecRecord temp = listHead;
	    
	    while (temp != null)
	    {
		if ((temp.type() == t) &&
		    (temp.getRecoveryStatus() != RecoveryStatus.COMPLETE))
		    count++;
		
		temp = temp.next;
	    }
	}
	
	return count;
    }

public void printWorkIds (PrintStream strm)
    {
	if (noEntries != 0)
	{
	    CrashRecRecord temp = listHead;
	    
	    while (temp != null)
	    {
		if (temp.getRecoveryStatus() != RecoveryStatus.COMPLETE)
		    temp.id(strm);
		
		temp = temp.next;
	    }
	}
    }

public boolean remove (CrashRecRecord p)
    {
	if (p.equals(listHead))
	    listHead = p.next;

	if (p.equals(listTail))
	    listTail = p.previous;

	p = null;
	noEntries--;

	return true;
    }

public void print (PrintStream strm)
    {
	if (noEntries != 0)
	{
	    strm.println("----------------------------------------");
	    strm.println("Number of entries in list = "+noEntries);

	    CrashRecRecord temp = listHead;
	    
	    while (temp != null)
	    {
		strm.println("----------------------------------------");
		temp.print(strm);
		temp = temp.next;
	    }
	    strm.println("----------------------------------------");
	}
	else
	    strm.println("** LIST IS EMPTY **");
    }

public boolean resolve ()
    {
	if (noEntries != 0)
	{
	    CrashRecRecord temp = listHead;

	    // Skip over the completed ones to find the first unresolved one
	    
	    while ( (temp != null) && (temp.getRecoveryStatus() == RecoveryStatus.COMPLETE) )
		temp = temp.next;

	    if (temp != null)
	    {
		CrashRecRecord firstWork = temp;
		boolean resolvedAll = true; // Resolved all of this type of record?

		if ( !(temp.resolve()) )
		    resolvedAll = false;
		temp = temp.next;

		while( (temp != null) && (temp.type() == firstWork.type()) )
		{
		    if (temp.getRecoveryStatus() != RecoveryStatus.COMPLETE)
		    {
			if ( !(temp.resolve()) )
			    resolvedAll = false;
		    }
		    temp = temp.next;
		}
		if ( (temp != null) && resolvedAll )
		    resolve();
	    }
	}
	
	return true;
    }

public boolean purge ()
    {
	if (noEntries != 0)
	{
	    CrashRecRecord temp = listHead;
	    
	    while (temp != null)
	    {
		if ( temp.getRecoveryStatus() == RecoveryStatus.COMPLETE )
		    remove(temp);

		temp = temp.next;
	    }
	}

	return true;
    }

private PrintStream outStrm;
private CrashRecRecord listHead;
private CrashRecRecord listTail;
private int noEntries;

}
