package javax.jaxtx.exceptions;

/**
 * Thrown if an attempt is made to utilise an unknown transaction.
 */

public class InvalidTransactionException extends JAXTXException
{

    public InvalidTransactionException ()
    {
	super();
    }

    public InvalidTransactionException (String s)
    {
	super(s);
    }

    public InvalidTransactionException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


