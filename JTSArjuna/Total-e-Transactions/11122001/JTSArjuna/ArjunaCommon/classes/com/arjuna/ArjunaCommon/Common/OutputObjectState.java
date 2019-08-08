/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OutputObjectState.java,v 1.1.8.1.22.1.4.2 2001/01/15 10:06:14 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import java.io.PrintWriter;

import java.io.IOException;

/**
 * OutputObjectState provides some additional methods to a
 * basic OutputBuffer.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OutputObjectState.java,v 1.1.8.1.22.1.4.2 2001/01/15 10:06:14 nmcl Exp $
 * @since JTS 1.0.
 */

public class OutputObjectState extends OutputBuffer
{

public OutputObjectState ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_BUFFER_MAN, "OutputObjectState::OutputObjectState()");
	}

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

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_BUFFER_MAN, "OutputObjectState::OutputObjectState("+copyFrom+")");
	}

	bufferUid = new Uid(copyFrom.bufferUid);
	super._valid = bufferUid.valid();
	
	imageType = new String(copyFrom.imageType);
    }

public OutputObjectState (InputObjectState copyFrom)
    {
	super(copyFrom.buffer());

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_BUFFER_MAN, "OutputObjectState::OutputObjectState("+copyFrom+")");
	}

	bufferUid = new Uid(copyFrom.stateUid());
	super._valid = bufferUid.valid();

	imageType = new String(copyFrom.type());
    }
    
public OutputObjectState (Uid newUid, String tName)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_BUFFER_MAN, "OutputObjectState::OutputObjectState("+newUid+", "+tName+")");
	}

	bufferUid = new Uid(newUid);
	super._valid = bufferUid.valid();
	
	imageType = new String(tName);
    }

public OutputObjectState (Uid newUid, String tName, byte[] buffer)
    {
	super(buffer);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_BUFFER_MAN, "OutputObjectState::OutputObjectState("+newUid+", "+tName+")");
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

public void print (PrintWriter strm)
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
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.OPERATORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_BUFFER_MAN, "OutputObjectState::copy for "+bufferUid);
	}

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

private Uid    bufferUid;
private String imageType;

}
