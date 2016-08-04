/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InputBuffer.java,v 1.2 1998/04/09 15:48:03 nmcl Exp $
 */

package com.arjuna.RPC;

import java.io.*;
import java.io.IOException;

public class InputBuffer
{
    
public InputBuffer (byte b[]) throws IOException
    {
	_inputStream = new ByteArrayInputStream(b);
	_input = new DataInputStream(_inputStream);

	skipHeader();
    }

public byte unpackByte () throws IOException
    {
	int i = unpackInt();
	
	return (byte) i;
    }

public byte[] unpackBytes () throws IOException
    {
	int index = unpackInt();
	int size = unpackInt();
	byte b[] = new byte[size];

	if (size > 0)
	{
	    _input.readFully(b);

	    realign(size);
	}
	
	return b;
    }
    
public boolean unpackBoolean () throws IOException
    {
	_input.skipBytes(3);

	return _input.readBoolean();
    }

public char unpackChar () throws IOException
    {
	int i = unpackInt();
	
	return (char) i;
    }

public short unpackShort () throws IOException
    {
	int i = unpackInt();

	return (short) i;
    }    

public int unpackInt () throws IOException
    {
	return _input.readInt();
    }

public long unpackLong () throws IOException
    {
	return _input.readLong();
    }

public float unpackFloat () throws IOException
    {
	return _input.readFloat();
    }

public double unpackDouble () throws IOException
    {
	return _input.readDouble();
    }

public String unpackString () throws IOException
    {
	int index = unpackInt();
	int length = unpackInt();

	/*
	 * We don't need the '\0' character which Arjuna puts in
	 * the buffer. We only put it in for compatibility with
	 * C++. So ignore it.
	 */
	
	byte[] b = new byte[length-1];
	byte[] dummy = new byte[1];
	
	_input.read(b, 0, length-1);
	_input.read(dummy, 0, 1);

	realign(length);

	return new String(b);
    }
    
private void realign (int amount) throws IOException
    {
	if ((amount % OutputBuffer.ALIGNMENT) > 0)
	{
	    int excess = OutputBuffer.ALIGNMENT - (amount % OutputBuffer.ALIGNMENT);

	    if (_inputStream.available() < excess)
		excess = _inputStream.available();
	
	    _input.skipBytes(excess);
	}
    }
    
private void skipHeader () throws IOException
    {
	_input.skipBytes(OutputBuffer.headerSize);  // sizeof buffer header
    }

private DataInputStream _input;
private ByteArrayInputStream _inputStream;
    
}
