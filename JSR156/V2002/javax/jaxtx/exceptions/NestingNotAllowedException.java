package javax.jaxtx.exceptions;

/**
 * Not all transaction models allow nesting of transactions to occur. This
 * exception is thrown if an attempt is made to create a new transaction
 * when a transaction is already associated with the invoking thread and
 * the transaction implementation does not support nesting.
 */

public class NestingNotAllowedException extends JAXTXException
{

    public NestingNotAllowedException ()
    {
	super();
    }

    public NestingNotAllowedException (String s)
    {
	super(s);
    }

    public NestingNotAllowedException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


