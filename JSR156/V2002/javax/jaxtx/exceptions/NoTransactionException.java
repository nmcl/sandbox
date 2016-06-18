package javax.jaxtx.exceptions;

/**
 * Thrown if there is no transaction associated with the invoking
 * thread and one needs to be in order to execute the operation.
 */

public class NoTransactionException extends JAXTXException
{

    public NoTransactionException ()
    {
	super();
    }

    public NoTransactionException (String s)
    {
	super(s);
    }

    public NoTransactionException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


