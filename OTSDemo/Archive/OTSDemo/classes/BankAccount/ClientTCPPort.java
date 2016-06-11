package BankAccount;

import java.io.*;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.io.IOException;

public class ClientTCPPort
{
    
public ClientTCPPort ()
    {
	_valid = true;
	_isConnected = false;
    }

protected void finalize ()
    {
	if (_isConnected)
	{
	    try
	    {
		_myOutputStream.flush();
	    	_myInputStream.close();
		_myOutputStream.close();
		_sock.close();
		_isConnected = false;
	    }
	    catch (IOException e)
	    {
	    }
	}
    }
    
public boolean valid ()
    {
	return _valid;
    }

public boolean connected ()
    {
	return _isConnected;
    }
    
public synchronized void sendMessage (OutputBuffer[] buff) throws IOException
    {
	if (!_isConnected)
	    throw(new IOException("ClientTCPPort not connected."));
	
	int buffSize = buff.length;
	int i;

	if (buffSize > SetupBuffer.MSG_MAXIOVLEN)
	    throw(new IOException("Too many OutputBuffers"));

	if (buffSize == 0)
	    throw(new IOException("Too few OutputBuffers"));
	
	SetupBuffer _setup = new SetupBuffer();

	for (i = 0; i < buffSize; i++)
	    _setup.bufferSize[i] = buff[i].length();

	_myOutputStream.write(_setup.pack());

	for (i = 0; i < buffSize; i++)
	    _myOutputStream.write(buff[i].buffer());

	_myOutputStream.flush();
    }

public synchronized InputBuffer[] receiveMessage () throws IOException
    {
	if (!_isConnected)
	    throw(new IOException("ClientTCPPort not connected."));

	SetupBuffer _setup = new SetupBuffer();
	byte[] _byte = new byte[SetupBuffer.headerSize];

	_myInputStream.read(_byte);
	
	_setup.unpack(_byte);

	int numberOfBuffers = 0;
	boolean finished = false;

	for (int i = 0; (i < SetupBuffer.MSG_MAXIOVLEN) && (!finished); i++)
	{
	    if (_setup.bufferSize[i] > 0)
		numberOfBuffers++;
	    else
		finished = true;
	}

	if (numberOfBuffers == 0)
	    throw(new IOException("Received invalid number of InputBuffers"));

	InputBuffer buff[] = new InputBuffer[numberOfBuffers];

	for (int j = 0; j < numberOfBuffers; j++)
	{
	    byte[] _data = new byte[_setup.bufferSize[j]];

	    _myInputStream.read(_data);

	    buff[j] = new InputBuffer(_data);
	}

	return buff;
    }

public synchronized void setupConnection (PortAddress addr) throws IOException
    {
	if (_isConnected)
	    throw(new IOException("Socket already connected."));

	if (!addr.valid())
	    throw(new IOException("Invalid PortAddress."));
	
	_sock = new Socket(addr.hostName(), addr.portNumber(), true);
	_myOutputStream = _sock.getOutputStream();
	_myInputStream = _sock.getInputStream();
	_isConnected = true;
    }

public synchronized void endConnection () throws IOException
    {
	if (!_isConnected)
	    throw(new IOException("Socket not connected."));

	_myOutputStream.flush();
	
	_myInputStream.close();
	_myOutputStream.close();
	_sock.close();
	_isConnected = false;
    }

public synchronized boolean pollForMessage () throws IOException
    {
	if (!_isConnected)
	    throw(new IOException("Socket not connected."));

	if (_myInputStream.available() > 0)
	    return true;
	else
	    return false;
    }

private boolean _valid;
private boolean _isConnected;
private Socket _sock;
private OutputStream _myOutputStream;
private InputStream _myInputStream;
    
}
