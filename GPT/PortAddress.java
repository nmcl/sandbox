import java.net.Socket;
import java.net.SocketException;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.io.IOException;
import java.io.*;

public class PortAddress
{

public PortAddress ()
    {
	_valid = false;
    }

public PortAddress (int port)
    {
	_valid = true;
	_port = port;

	try
	{
	    InetAddress inet = InetAddress.getLocalHost();
	    _hostName = inet.getHostName();
	}
	catch (UnknownHostException e)
	{
	    _valid = false;
	};
    }

public PortAddress (String host, int port)
    {
	_valid = true;
	_port = port;
	_hostName = host.toString();
    }

public boolean valid ()
    {
	return _valid;
    }

public boolean assign (PortAddress addr)
    {
	_valid = addr.valid();
	_port = addr.portNumber();
	_hostName = addr.hostName().toString();

	return true;
    }

public boolean equals (PortAddress addr)
    {
	if (_valid && addr.valid())
	{
	    if ((_port == addr.portNumber()) && _hostName.equals(addr.hostName()))
		return true;
	}

	return false;
    } 

public boolean notequals (PortAddress addr)
    {
	if (this.equals(addr))
	    return false;
	else
	    return true;
    }

public void pack (OutputBuffer buff) throws IOException
    {
	buff.packBoolean(_valid);
	buff.packInt(_port);
	buff.packString(_hostName);
    }

public void unpack (InputBuffer buff) throws IOException
    {
	_valid = buff.unpackBoolean();
	_port = buff.unpackInt();
	_hostName = buff.unpackString();
    }

public void print ()
    {
	System.out.println("PortAddress < "+_valid+", "+_port+", "+_hostName+" >\n");
    }
    
public int portNumber ()
    {
	return _port;
    }

public String hostName ()
    {
	return _hostName;
    }

private boolean _valid;
private int _port;
private String _hostName;

}
