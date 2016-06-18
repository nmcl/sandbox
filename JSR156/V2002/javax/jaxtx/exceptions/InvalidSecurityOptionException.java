package javax.jaxtx.exceptions;

/**
 * The invoker does not have sufficient access rights to attempt the
 * operation. For example, obtaining all of the coordinator implementations
 * that are provided by the underlying implementation.
 */

public class InvalidSecurityOptionException extends JAXTXException
{

    public InvalidSecurityOptionException ()
    {
	super();
    }

    public InvalidSecurityOptionException (String s)
    {
	super(s);
    }

    public InvalidSecurityOptionException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}
