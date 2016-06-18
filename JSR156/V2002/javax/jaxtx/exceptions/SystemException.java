package javax.jaxtx.exceptions;

/**
 * Thrown if an error occurs which is not met by another specific
 * exception.
 */

public class SystemException extends JAXTXException
{

    public SystemException ()
    {
	super();
    }

    public SystemException (String s)
    {
	super(s);
    }

    public SystemException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}
