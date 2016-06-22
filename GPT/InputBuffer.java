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
	    
	byte[] b = new byte[length];
	_input.read(b, 0, length);

	return new String(b);
    }

private void skipHeader () throws IOException
    {
	_input.skipBytes(OutputBuffer.headerSize);  // sizeof buffer header
    }
    
private DataInputStream _input;
private ByteArrayInputStream _inputStream;
    
}
