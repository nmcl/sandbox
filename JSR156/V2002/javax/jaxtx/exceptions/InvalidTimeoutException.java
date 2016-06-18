package javax.jaxtx.exceptions;

/**
 * Thrown if the timeout value associated with the invoking thread
 * which should be applied to newly created transactions is invalid.
 */

public class InvalidTimeoutException extends JAXTXException
{

    public InvalidTimeoutException ()
    {
	super();
    }

    public InvalidTimeoutException (String s)
    {
	super(s);
    }

    public InvalidTimeoutException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


