/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: InputObjectState.java,v 1.1.8.1.22.1.4.2 2001/01/15 10:06:14 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import java.io.PrintWriter;

import java.io.IOException;

/**
 * InputObjectState provides some additional methods to
 * a basic InputBuffer.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: InputObjectState.java,v 1.1.8.1.22.1.4.2 2001/01/15 10:06:14 nmcl Exp $
 * @since JTS 1.0.
 */

public class InputObjectState extends InputBuffer
{

public InputObjectState ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_BUFFER_MAN, "InputObjectState::InputObjectState()");
	}
	
	bufferUid = new Uid(Uid.nullUid());
	super._valid = false;
	imageType = null;
    }
    
public InputObjectState (InputObjectState copyFrom)
    {
	super(copyFrom);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_BUFFER_MAN, "InputObjectState::InputObjectState("+copyFrom+")");
	}

	bufferUid = new Uid(copyFrom.bufferUid);
	super._valid = bufferUid.valid();
	
	imageType = new String(copyFrom.imageType);
    }

public InputObjectState (OutputObjectState copyFrom)
    {
	super(copyFrom.buffer());

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_BUFFER_MAN, "InputObjectState::InputObjectState("+copyFrom+")");
	}

	bufferUid = new Uid(copyFrom.stateUid());
	super._valid = bufferUid.valid();
	
	imageType = new String(copyFrom.type());
    }
    
public InputObjectState (Uid newUid, String tName, byte[] buff)
    {
	super(buff);  // implicitly copies the array contents.

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_BUFFER_MAN, "InputObjectState::InputObjectState("+newUid+", "+tName+")");
	}

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
    
public void print (PrintWriter strm)
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
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.OPERATORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_BUFFER_MAN, "InputObjectState::copy for "+bufferUid);
	}

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

private Uid    bufferUid;
private String imageType;

}
