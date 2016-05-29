package BankAccount;

import java.io.*;
import java.io.IOException;

public class OutputBuffer
{
    
public OutputBuffer (int buffSize) throws IOException
    {
	_outputStream = new ByteArrayOutputStream(buffSize);
	_output = new DataOutputStream(_outputStream);
	_byte = new byte[1];

	initBuffer();
    }

public OutputBuffer () throws IOException
    {
	_outputStream = new ByteArrayOutputStream();
	_output = new DataOutputStream(_outputStream);
	_byte = new byte[1];

	initBuffer();
    }

public byte[] buffer ()
    {
	return _outputStream.toByteArray();
    }

public int length ()
    {
	return _outputStream.size();
    }

public void reset () throws IOException
    {
	_outputStream.reset();
	initBuffer();
    }
    
public void packBoolean (boolean b) throws IOException
    {
	for (int i = 0; i < 3; i++)
	    _output.write(_byte, 0, 1);
	
	_output.writeBoolean(b);
	_output.flush();
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
    }

public void packLong (long l) throws IOException
    {
	_output.writeLong(l);
	_output.flush();	    
    }

public void packFloat (float f) throws IOException
    {
	_output.writeFloat(f);
	_output.flush();	    
    }

public void packDouble (double d) throws IOException
    {
	_output.writeDouble(d);
	_output.flush();	    
    }

public void packString (String s) throws IOException
    {
	int index = -1;
	int sz = s.length()+1;
	String dummy = s+'\0';
	
	packInt(index);
	packInt(sz);

	_output.writeBytes(dummy);
	_output.flush();
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
    
private DataOutputStream _output;
private DataInputStream _input;
private ByteArrayOutputStream _outputStream;
private ByteArrayInputStream _inputStream;
private byte[] _byte;
    
}
