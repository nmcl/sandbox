/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OutputBuffer.java,v 1.1.8.1.22.1.4.2 2001/01/15 10:06:14 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import java.io.*;

import java.io.IOException;

/**
 * An OuptputBuffer is used to store various Java types as a byte
 * stream. Similar to java serialization. However, OutputBuffers
 * are compatible with OTSArjuna states.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OutputBuffer.java,v 1.1.8.1.22.1.4.2 2001/01/15 10:06:14 nmcl Exp $
 * @since JTS 1.0.
 */

public class OutputBuffer
{

    /**
     * Create a new buffer.
     */

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

    /**
     * Create a new buffer with the specified initial size. If required,
     * the internal byte array will be automatically increased in size.
     */

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

    /**
     * Create a new buffer using the provided byte array.
     */

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

    /**
     * Create a new OutputBuffer and initialise its state with a
     * copy of the provided buffer.
     */

public OutputBuffer (OutputBuffer copyFrom)
    {
	_valid = true;
	_outputStream = null;
	_output = null;
	    
	copy(copyFrom);
    }

    /**
     * Is the buffer valid?
     */

public final synchronized boolean valid ()
    {
	return _valid;
    }

    /**
     * Return the byte array used to store data types.
     */

public final synchronized byte[] buffer ()
    {
	return _outputStream.toByteArray();
    }

    /**
     * Return the length of the byte array being used to store
     * data types.
     */

public final synchronized int length ()
    {
	return _outputStream.size();
    }

    /**
     * Copy the provided OutputBuffer and overwrite the current
     * instance.
     */

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

    /**
     * Clear the OutputBuffer and rewind the pack pointer.
     */

public final synchronized void reset () throws IOException
    {
	_outputStream.reset();
	initBuffer();
    }

    /**
     * Pack a byte.
     * If the buffer is invalid then an IOException is thrown.
     */

public final synchronized void packByte (byte b) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: byte.");
	
	packInt((byte) b);
    }

    /**
     * Pack the array of bytes.
     * If the buffer is invalid then an IOException is thrown.
     */

public final synchronized void packBytes (byte[] b) throws IOException
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

    /**
     * Pack the boolean.
     * If the buffer is invalid then an IOException is thrown.
     */

public final synchronized void packBoolean (boolean b) throws IOException
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

    /**
     * Pack the character.
     * If the buffer is invalid then an IOException is thrown.
     */

public final synchronized void packChar (char c) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: char.");
	
	packInt((int) c);
    }

    /**
     * Pack the short.
     * If the buffer is invalid then an IOException is thrown.
     */

public final synchronized void packShort (short s) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: short.");
	
	packInt((int) s);
    }    

    /**
     * Pack the integer.
     * If the buffer is invalid then an IOException is thrown.
     */

public final synchronized void packInt (int i) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: int.");
	
	_valid = false;
	
	_output.writeInt(i);
	_output.flush();

	_valid = true;
    }

    /**
     * Pack the long.
     * If the buffer is invalid then an IOException is thrown.
     */

public final synchronized void packLong (long l) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: long.");
	
	_valid = false;
	
	_output.writeLong(l);
	_output.flush();

	_valid = true;
    }

    /**
     * Pack the float.
     * If the buffer is invalid then an IOException is thrown.
     */

public final synchronized void packFloat (float f) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: float.");
	
	_valid = false;
	
	_output.writeFloat(f);
	_output.flush();

	_valid = true;
    }

    /**
     * Pack the double.
     * If the buffer is invalid then an IOException is thrown.
     */

public final synchronized void packDouble (double d) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: double.");
	
	_valid = false;
	
	_output.writeDouble(d);
	_output.flush();

	_valid = true;
    }

    /**
     * Pack the String.
     *
     * Currently different from the C++ version in that
     * a copy of the string will always be packed, even if
     * we have previously seen this object.
     *
     * If the buffer is invalid then an IOException is thrown.
     */

public final synchronized void packString (String s) throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid output buffer: string.");

	int index = -1;
	int sz = 0;
	String dummy = null;
	
	packInt(index);
	
	if (s != null)
	{
	    sz = s.length()+1;
	    dummy = s+'\0';

	    packInt(sz);
	}
	else
	{
	    packInt(0);
	}

	_valid = false;

	if (sz > 0)
	{
	    _output.write(dummy.getBytes(), 0, dummy.getBytes().length);
	    realign(dummy.getBytes().length);
	}
	
	_output.flush();

	_valid = true;
    }

    /**
     * Pack this buffer into that provided.
     * If the buffer is invalid then an IOException is thrown.
     */

public synchronized void packInto (OutputBuffer buff) throws IOException
    {
	if (buff == null)
	    throw new IOException("Invalid into buffer");
	
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

    /**
     * Print out information about this instance.
     */

public void print (PrintWriter strm)
    {
	if (_valid)
	{
	    strm.println("OutputBuffer : \n");

	    byte[] b = buffer();
	    
	    for (int i = 0; i < b.length; i++)
		strm.write((char) b[i]);
	}
	else
	    strm.println("OutputBuffer : invalid.");	
    }

    /**
     * Reset the pack pointer.
     */

public final boolean rewrite ()
    {
	if (!_valid)
	    return false;
	
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

	return _valid;
    }

    /*
     * 1 = 3
     */
    
private final void realign (int amount) throws IOException
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
protected static final int ALIGNMENT = 4;
    
private DataOutputStream      _output;
private DataInputStream       _input;
private ByteArrayOutputStream _outputStream;
private ByteArrayInputStream  _inputStream;
    
private static final byte[] _byte = new byte[1];
    
}
