package javax.jaxtx.exceptions;

/**
 * Thrown if a request for a specific transaction implementation cannot
 * be met. For example, an application asks for an ACID transaction
 * implementation based on JTA and only JTS is provided.
 */

public class NoSuchServiceException extends JAXTXException
{

    public NoSuchServiceException ()
    {
	super();
    }

    public NoSuchServiceException (String s)
    {
	super(s);
    }

    public NoSuchServiceException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


