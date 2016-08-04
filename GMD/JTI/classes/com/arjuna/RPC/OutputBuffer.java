/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OutputBuffer.java,v 1.2 1998/04/09 15:48:04 nmcl Exp $
 */

package com.arjuna.RPC;

import java.io.*;
import java.io.IOException;

public class OutputBuffer
{
    
public OutputBuffer (int buffSize) throws IOException
    {
	_havePacked = false;
	_outputStream = new ByteArrayOutputStream(buffSize);
	_output = new DataOutputStream(_outputStream);
	_byte = new byte[1];

	initBuffer();
    }

public OutputBuffer () throws IOException
    {
	_havePacked = false;	
	_outputStream = new ByteArrayOutputStream();
	_output = new DataOutputStream(_outputStream);
	_byte = new byte[1];

	initBuffer();
    }

public byte[] buffer ()
    {
	if (_havePacked)
	    return _outputStream.toByteArray();
	else
	    return null;
    }

public int length ()
    {
	if (_havePacked)
	    return _outputStream.size();
	else
	    return 0;
    }

public void reset () throws IOException
    {
	_outputStream.reset();
	initBuffer();
    }

public void packByte (byte b) throws IOException
    {
	packInt((int) b);
    }

public void packBytes (byte[] b) throws IOException
    {
	int index = -1;
	
	packInt(index);
	packInt(b.length);

	if (b.length > 0)
	{
	    _output.write(b, 0, b.length);
	    realign(b.length);
	}
	
	_output.flush();
    }	
    
public void packBoolean (boolean b) throws IOException
    {
	for (int i = 0; i < 3; i++)
	    _output.write(_byte, 0, 1);
	
	_output.writeBoolean(b);
	_output.flush();
	_havePacked = true;
    }

public void packChar (char c) throws IOException
    {
	packInt((int) c);
    }

public void packShort (short s) throws IOException
    {
	packInt((int) s);
    }    

public void packInt (int i) throws IOException
    {
	_output.writeInt(i);
	_output.flush();
	_havePacked = true;
    }

public void packLong (long l) throws IOException
    {
	_output.writeLong(l);
	_output.flush();
	_havePacked = true;
    }

public void packFloat (float f) throws IOException
    {
	_output.writeFloat(f);
	_output.flush();
	_havePacked = true;
    }

public void packDouble (double d) throws IOException
    {
	_output.writeDouble(d);
	_output.flush();
	_havePacked = true;
    }

public void packString (String s) throws IOException
    {
	int index = -1;
	int sz = s.length()+1;
	String dummy = s+'\0';

	packInt(index);
	packInt(sz);

	_output.write(dummy.getBytes(), 0, dummy.getBytes().length);
	realign(dummy.getBytes().length);
	
	_output.flush();
	_havePacked = true;
    }

    /*
     * 1 = 3
     */
    
private void realign (int amount) throws IOException
    {
	if ((amount % ALIGNMENT) > 0)
	{
	    int excess = ALIGNMENT - (amount % ALIGNMENT);

	    for (int i = 0; i < excess; i++)
		_output.write(_byte, 0, 1);
	    
	    _output.flush();
	}
    }
    
private void initBuffer () throws IOException
    {
	String version = "#BE";

	_output.writeBytes(version);
	_output.writeBoolean(true);
	_output.writeByte(16);
	_output.writeByte(32);
	_output.writeByte(64);
	_output.writeByte(0);
    }

protected final static int headerSize = 8;
protected final static int ALIGNMENT = 4;
    
private DataOutputStream _output;
private DataInputStream _input;
private ByteArrayOutputStream _outputStream;
private ByteArrayInputStream _inputStream;
private byte[] _byte;
private boolean _havePacked;
    
}
