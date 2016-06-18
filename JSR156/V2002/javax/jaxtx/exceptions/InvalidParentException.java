package javax.jaxtx.exceptions;

/**
 * The parent transaction was invalid in the scope it was intended to be
 * used.
 */

public class InvalidParentException extends JAXTXException
{

    public InvalidParentException ()
    {
	super();
    }

    public InvalidParentException (String s)
    {
	super(s);
    }

    public InvalidParentException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


