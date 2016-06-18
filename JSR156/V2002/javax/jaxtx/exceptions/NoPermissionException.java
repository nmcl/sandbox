package javax.jaxtx.exceptions;

/**
 * The invoking thread does not have permission to attempt to use the
 * operation. For example, some transaction implementations only allow
 * the creating thread to terminate a transaction.
 *
 * Do we want to remove this and replace it with SecurityException, as
 * the JTA has done?
 */

public class NoPermissionException extends JAXTXException
{

    public NoPermissionException ()
    {
	super();
    }

    public NoPermissionException (String s)
    {
	super(s);
    }

    public NoPermissionException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


