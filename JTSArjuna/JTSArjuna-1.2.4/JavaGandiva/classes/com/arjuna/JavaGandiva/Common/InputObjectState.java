/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InputObjectState.java,v 1.4.8.1 1999/06/23 13:52:12 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Common;

import java.io.PrintStream;

import java.io.IOException;

public class InputObjectState extends InputBuffer
{

public InputObjectState ()
    {
	bufferUid = new Uid(Uid.nullUid());
	super._valid = false;
	imageType = null;
    }
    
public InputObjectState (InputObjectState copyFrom)
    {
	super(copyFrom);

	bufferUid = new Uid(copyFrom.bufferUid);
	super._valid = bufferUid.valid();
	
	imageType = new String(copyFrom.imageType);
    }

public InputObjectState (OutputObjectState copyFrom)
    {
	super(copyFrom.buffer());

	bufferUid = new Uid(copyFrom.stateUid());
	super._valid = bufferUid.valid();
	
	imageType = new String(copyFrom.type());
    }
    
public InputObjectState (Uid newUid, String tName, byte[] buff)
    {
	super(buff);  // implicitly copies the array contents.

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

public void copyFrom (OutputObjectState copyFrom)
    {
	super.setBuffer(copyFrom.buffer());

	bufferUid = new Uid(copyFrom.stateUid());
	super._valid = bufferUid.valid();
	
	imageType = new String(copyFrom.type());
    }
    
public void print (PrintStream strm)
    {
	strm.println("InputObjectState Uid   : "+bufferUid+"\n");

	if (imageType != null)
	    strm.println("InputObjectState Type  : "+imageType+"\n");
	else
	    strm.println("InputObjectState Type  : null\n");

	strm.println("InputObjectState Size  : "+size()+"\n");
	strm.println("InputObjectState Buffer: ");

	super.print(strm);
    }
    
public synchronized void copy (InputObjectState objstate)
    {
	super.copy(objstate);

	bufferUid = new Uid(objstate.bufferUid);
	super._valid = bufferUid.valid();
	
	imageType = new String(objstate.imageType);
    }

public synchronized void unpackFrom (InputBuffer buff) throws IOException
    {
	imageType = buff.unpackString();
	bufferUid.unpack(buff);

	super.unpackFrom(buff);
    }

private Uid bufferUid;
private String imageType;

}
