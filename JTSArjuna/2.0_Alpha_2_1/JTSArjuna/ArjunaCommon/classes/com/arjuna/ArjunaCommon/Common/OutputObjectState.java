/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OutputObjectState.java,v 1.1 2000/02/25 14:02:39 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import java.io.PrintStream;

import java.io.IOException;

public class OutputObjectState extends OutputBuffer
{

public OutputObjectState ()
    {
	bufferUid = new Uid();
	super._valid = bufferUid.valid();
	
	imageType = null;
    }
    
public OutputObjectState (OutputObjectState copyFrom)
    {
	/*
	 * Don't use byte[] constructor as buffer is already
	 * initialised.
	 */
	
	super(copyFrom);

	bufferUid = new Uid(copyFrom.bufferUid);
	super._valid = bufferUid.valid();
	
	imageType = new String(copyFrom.imageType);
    }

public OutputObjectState (InputObjectState copyFrom)
    {
	super(copyFrom.buffer());

	bufferUid = new Uid(copyFrom.stateUid());
	super._valid = bufferUid.valid();

	imageType = new String(copyFrom.type());
    }
    
public OutputObjectState (Uid newUid, String tName)
    {
	bufferUid = new Uid(newUid);
	super._valid = bufferUid.valid();
	
	imageType = new String(tName);
    }

public OutputObjectState (Uid newUid, String tName, byte[] buffer)
    {
	super(buffer);

	bufferUid = new Uid(newUid);
	super._valid = bufferUid.valid();
	
	imageType = new String(tName);
    }

public final boolean notempty ()
    {
	return ((length() > 0) ? true : false);
    }

public final int size ()
    {
	return (length());
    }

public final Uid stateUid ()
    {
	return bufferUid;
    }

public final String type ()
    {
	return imageType;
    }

public void print (PrintStream strm)
    {
	strm.println("OutputObjectState Uid   : "+bufferUid+"\n");
	
	if (imageType != null)
	    strm.println("OutputObjectState Type  : "+imageType+"\n");
	else
	    strm.println("OutputObjectState Type  : null\n");

	strm.println("OutputObjectState Size  : "+size()+"\n");
	strm.println("OutputObjectState Buffer: ");

	super.print(strm);
    }

public synchronized void copy (OutputObjectState objstate)
    {
	super.copy(objstate);

	bufferUid = new Uid(objstate.bufferUid);
	super._valid = bufferUid.valid();
	
	imageType = new String(objstate.imageType);
    }
    
public synchronized void packInto (OutputBuffer buff) throws IOException
    {
	buff.packString(imageType);
	bufferUid.pack(buff);

	super.packInto(buff);
    }

private Uid bufferUid;
private String imageType;

}
