/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OutputBuffer.java,v 1.7.2.1 1999/02/17 15:19:52 nmcl Exp $
 */

package com.arjuna.JavaArjuna.Common;

import java.io.*;

import java.io.IOException;

public class OutputBuffer
{
    
public OutputBuffer ()
    {
	_valid = true;

	try
	{
	    _outputStream = new ByteArrayOutputStream();
	    _output = new DataOutputStream(_outputStream);
	
	    initBuffer();
	}
	catch (IOException e)
	{
	    _valid = false;
	}
    }

public OutputBuffer (int buffSize)
    {
	_valid = true;

	try
	{
	    _outputStream = new ByteArrayOutputStream(buffSize);
	    _output = new DataOutputStream(_outputStream);

	    initBuffer();
	}
	catch (IOException e)
	{
	    _valid = false;
	}
    }

public OutputBuffer (byte[] b)
    {
	_valid = true;

	try
	{
	    _outputStream = new ByteArrayOutputStream(0);
	    _output = new DataOutputStream(_outputStream);

	    _outputStream.write(b, 0, b.length);
	    
	    initBuffer();
	}
	catch (IOException e)
	{
	    _valid = false;
	}
    }

public OutputBuffer (OutputBuffer copyFrom)
    {
	_valid = true;
	_outputStream = null;
	_output = null;
	    
	copy(copyFrom);
    }

public final synchronized boolean valid ()
    {
	return _valid;
    }
    
public synchronized byte[] buffer ()
    {
	return _outputStream.toByteArray();
    }

public synchronized int length ()
    {
	return _outputStream.size();
    }

public synchronized void copy (OutputBuffer b)
    {
	if (b._valid)
	{
	    _valid = true;

	    try
	    {
		_outputStream = new ByteArrayOutputStream(b.length());
		_output = new DataOutputStream(_outputStream);

		_outputStream.write(b.buffer(), 0, b.length());
	    
		initBuffer();
	    }
	    catch (IOException e)
	    {
		_valid = false;
	    }
	}
    }

public synchronized void reset () throws IOException
    {
	_outputStream.reset();
	initBuffer();
    }

public synchronized void packByte (byte b) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: byte.");
	
	packInt((byte) b);
    }

public synchronized void packBytes (byte[] b) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: bytes.");

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
   
public synchronized void packBoolean (boolean b) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: boolean.");
	
	_valid = false;
	
	for (int i = 0; i < 3; i++)
	    _output.write(OutputBuffer._byte, 0, 1);
	
	_output.writeBoolean(b);
	_output.flush();

	_valid = true;
    }

public synchronized void packChar (char c) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: char.");
	
	packInt((int) c);
    }

public synchronized void packShort (short s) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: short.");
	
	packInt((int) s);
    }    

public synchronized void packInt (int i) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: int.");
	
	_valid = false;
	
	_output.writeInt(i);
	_output.flush();

	_valid = true;
    }

public synchronized void packLong (long l) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: long.");
	
	_valid = false;
	
	_output.writeLong(l);
	_output.flush();

	_valid = true;
    }

public synchronized void packFloat (float f) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: float.");
	
	_valid = false;
	
	_output.writeFloat(f);
	_output.flush();

	_valid = true;
    }

public synchronized void packDouble (double d) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: double.");
	
	_valid = false;
	
	_output.writeDouble(d);
	_output.flush();

	_valid = true;
    }

public synchronized void packString (String s) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: string.");
	
	int index = -1;
	int sz = s.length()+1;
	String dummy = s+'\0';

	packInt(index);
	packInt(sz);

	_valid = false;
	
	_output.write(dummy.getBytes(), 0, dummy.getBytes().length);
	realign(dummy.getBytes().length);
	
	_output.flush();

	_valid = true;
    }

public synchronized void packInto (OutputBuffer buff) throws IOException
    {
	if (_valid)
	{
	    /*
	     * pack number of bytes and then pack each byte separately.
	     */

	    byte[] b = buffer();
	    buff.packInt(b.length);

	    for (int i = 0; i < b.length; i++)
		buff.packByte(b[i]);
	}
    }
    
public void print (PrintStream strm)
    {
	if (_valid)
	{
	    strm.println("OutputBuffer : \n");
	    strm.write(buffer(), 0, length());
	}
	else
	    strm.println("OutputBuffer : invalid.");	
    }
    
    /*
     * 1 = 3
     */
    
private void realign (int amount) throws IOException
    {
	if ((amount % OutputBuffer.ALIGNMENT) > 0)
	{
	    int excess = OutputBuffer.ALIGNMENT - (amount % OutputBuffer.ALIGNMENT);

	    for (int i = 0; i < excess; i++)
		_output.write(_byte, 0, 1);
	    
	    _output.flush();
	}
    }
    
private final void initBuffer () throws IOException
    {
	String version = "#BE";

	_output.writeBytes(version);
	_output.writeBoolean(true);
	_output.writeByte(16);
	_output.writeByte(32);
	_output.writeByte(64);
	_output.writeByte(0);
    }

protected boolean _valid;
    
protected static final int headerSize = 8;
protected final static int ALIGNMENT = 4;
    
private DataOutputStream _output;
private DataInputStream _input;
private ByteArrayOutputStream _outputStream;
private ByteArrayInputStream _inputStream;
    
private static final byte[] _byte = new byte[1];
    
}
