/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Dozer.java,v 1.2 1998/07/06 10:49:51 nmcl Exp $
 */

package com.arjuna.JavaCosRecovery.CrashMan;

import java.io.PrintStream;
import java.util.Date;
import java.lang.InterruptedException;

/*
 * Default visibility.
 */

/*
 * Class: Dozer
 * Clever sleep with exponential backoff and possible threshold function
 * firing
 */

class Dozer
{
    
public Dozer (PrintStream strm)
    {
	outStrm = strm;
	baseSleepTime = 1;
	currentSleepTime = 1;
	threshold = -1;
	noTries = 0;
	firedWarning = false;
	thresholdObject = null;
    }

public Dozer (PrintStream strm, int initialSleepTime)
    {
	outStrm = strm;	
	baseSleepTime = initialSleepTime;
	currentSleepTime = initialSleepTime;
	threshold = -1;
	noTries = 0;
	firedWarning = false;
	thresholdObject = null;
    }
    
public void doze ()
    {
	// Check threshold

	if ( (noTries == threshold) && (thresholdObject != null) )
	{
	    if (thresholdObject.reached())
	    {
		// if thresholdObject returns true then reset noTries
		noTries = 0;
	    }
	    
	    firedWarning = true;
	}
	else
	{
	    sleep(currentSleepTime);
	    noTries++;
	    currentSleepTime = 2 * currentSleepTime;
	}
    }
        
public void reset ()
    {
	noTries = 0;
	currentSleepTime = baseSleepTime;
	firedWarning = false;
    }

public void reset (int newBase)
    {
	currentSleepTime = baseSleepTime = newBase;
	noTries = 0;
	currentSleepTime = baseSleepTime;
	firedWarning = false;
    }

public void setThreshold (int newThreshold)
    {
	threshold = newThreshold;
    }

public void setThresholdObject (ThresholdObject obj)
    {
	thresholdObject = obj;
    }

public boolean fired ()
    {
	return firedWarning;
    }
    
private void sleep (int sleepTime)
    {
	Date date = new Date();

	outStrm.println("Dozer: Sleeping for "+sleepTime+" secs. Its now " 
			+date.toGMTString());

	date = null;

	try
	{
	    Thread.sleep(sleepTime*1000);
	}
	catch (InterruptedException e)
	{
	}

	date = new Date();
	
	outStrm.println("Dozer: Awake at "+date.toGMTString());
    }
    
private PrintStream outStrm;
private int baseSleepTime;
private int currentSleepTime;
private int threshold;
private int noTries;
private boolean firedWarning;
private ThresholdObject thresholdObject;

}
