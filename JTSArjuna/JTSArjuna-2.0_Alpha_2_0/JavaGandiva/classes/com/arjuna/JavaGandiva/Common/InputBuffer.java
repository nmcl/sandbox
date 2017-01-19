/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InputBuffer.javatmpl,v 1.8 1999/09/10 10:24:58 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Common;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.PrintStream;

import java.io.IOException;

public class InputBuffer
{

public InputBuffer ()
    {
	_byteArray = null;
	_valid = false;
	_inputStream = null;
	_input = null;
    }

    /*
     * Create out own copy of the byte array.
     */
    
public InputBuffer (byte b[])
    {
	_valid = true;

	_byteArray = new byte[b.length];

	for (int i = 0; i < b.length; i++)
	    _byteArray[i] = b[i];
	
	try
	{
	    _inputStream = new ByteArrayInputStream(_byteArray);
	    _input = new DataInputStream(_inputStream);

	    skipHeader();
	}
	catch (IOException e)
	{
	    _valid = false;
	}
    }

public InputBuffer (InputBuffer buff)
    {
	_byteArray = null;
	_valid = false;
	_inputStream = null;
	_input = null;
	
	copy(buff);
    }

public synchronized boolean valid ()
    {
	return _valid;
    }
    
public synchronized void copy (InputBuffer buff)
    {
	if (buff._valid)
	{
	    _byteArray = new byte[buff._byteArray.length];
	    _valid = true;
	    
	    for (int i = 0; i < buff._byteArray.length; i++)
		_byteArray[i] = buff._byteArray[i];

	    try
	    {
		_inputStream = new ByteArrayInputStream(_byteArray);
		_input = new DataInputStream(_inputStream);

		skipHeader();
	    }
	    catch (IOException e)
	    {
		_valid = false;
	    }
	}
    }

public synchronized int length () 
    {
	return _byteArray.length;
    }

public synchronized byte[] buffer ()
    {
	return _byteArray;
    }

public synchronized void setBuffer (byte[] b)
    {
	_byteArray = new byte[b.length];

	for (int i = 0; i < b.length; i++)
	    _byteArray[i] = b[i];

	try
	{
	    _inputStream = new ByteArrayInputStream(_byteArray);
	    _input = new DataInputStream(_inputStream);

	    _valid = true;

	    skipHeader();
	}
	catch (Exception e)
	{
	    _byteArray = null;
	    _valid = false;
	}
    }

public synchronized byte unpackByte () throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid input buffer: byte.");
	
	int i = unpackInt();

	return (byte) i;
    }

public synchronized byte[] unpackBytes () throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid input buffer: bytes.");

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

public synchronized boolean unpackBoolean () throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid input buffer: boolean.");

	_valid = false;
	
	_inputStream.skip(3);

	boolean b = _input.readBoolean();

	_valid = true;

	return b;
    }

public synchronized char unpackChar () throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid input buffer: char.");
	
	int i = unpackInt();
	
	return (char) i;
    }

public synchronized short unpackShort () throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid input buffer: short.");
	
	int i = unpackInt();

	return (short) i;
    }    

public synchronized int unpackInt () throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid input buffer: int.");

	_valid = false;

	int i = _input.readInt();

	_valid = true;

	return i;
    }

public synchronized long unpackLong () throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid input buffer: long.");

	_valid = false;

	long l = _input.readLong();

	_valid = true;

	return l;
    }

public synchronized float unpackFloat () throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid input buffer: float.");

	_valid = false;

	float f = _input.readFloat();

	_valid = true;

	return f;
    }

public synchronized double unpackDouble () throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid input buffer: double.");

	_valid = false;

	double d = _input.readDouble();

	_valid = true;

	return d;
    }

    /**
     * Currently different from the ArjunaLite version in that
     * a distinct new instance will always be returned, rather
     * than a reference to a previously returned object in the
     * case of the "same" string.
     */
    
public synchronized String unpackString () throws IOException
    {
	if (!_valid)
	    throw new IOException("Invalid input buffer: string.");
	
	int index = unpackInt();
	int length = unpackInt();

	if (length == 0)
	    return null;
	
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

public synchronized void unpackFrom (InputBuffer buff) throws IOException
    {
	if (buff == null)
	    throw new IOException("Invalid from buffer");
	
	_valid = false;
	
	/*
	 * unpack number of bytes, then create new byte array
	 * and unpack each byte separately.
	 */

	int i = buff.unpackInt();

	_byteArray = new byte[i];

	for (int j = 0; j < i; j++)
	    _byteArray[j] = buff.unpackByte();

	_valid = true;
	    
	try
	{
	    _inputStream = new ByteArrayInputStream(_byteArray);
	    _input = new DataInputStream(_inputStream);

	    skipHeader();
	}
	catch (IOException e)
	{
	    _valid = false;
	}
    }

public void print (PrintStream strm)
    {
	if (_valid)
	{
	    strm.println("InputBuffer : \n");
	    strm.write(_byteArray, 0, _byteArray.length);
	}
	else
	    strm.println("InputBuffer : invalid.");
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
	_inputStream.skip(OutputBuffer.headerSize);  // sizeof buffer header
    }

protected boolean _valid;
    
private DataInputStream _input;
private ByteArrayInputStream _inputStream;
private byte[] _byteArray;    
    
}
