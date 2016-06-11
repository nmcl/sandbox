package BankAccount;

import java.io.*;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.io.IOException;

class SetupBuffer
{
    
public SetupBuffer ()
    {
	bufferSize = new int [SetupBuffer.MSG_MAXIOVLEN];
	
	for (int i = 0; i < SetupBuffer.MSG_MAXIOVLEN; i++)
	    bufferSize[i] = 0;

	packetNumber = 0;
	endOfMessage = 0;
    }

public byte[] pack () throws IOException
    {
	_outputStream = new ByteArrayOutputStream();
	_output = new DataOutputStream(_outputStream);

	for (int i = 0; i < SetupBuffer.MSG_MAXIOVLEN; i++)
	    _output.writeInt(bufferSize[i]);

	_output.writeInt(packetNumber);
	_output.writeInt(endOfMessage);

	return _outputStream.toByteArray();
    }

public void unpack (byte[] buff) throws IOException
    {
	_inputStream = new ByteArrayInputStream(buff);
	_input = new DataInputStream(_inputStream);

	for (int i = 0; i < SetupBuffer.MSG_MAXIOVLEN; i++)
	    bufferSize[i] = _input.readInt();

	packetNumber = _input.readInt();
	endOfMessage = _input.readInt();
    }
    
public int bufferSize[];
public int packetNumber;
public int endOfMessage;
    
public final static int MSG_MAXIOVLEN = 16;
public final static int headerSize = 72;  // bytes

private DataOutputStream _output;
private DataInputStream _input;
private ByteArrayOutputStream _outputStream;
private ByteArrayInputStream _inputStream;
    
}
