package javax.jaxtx.exceptions;

/**
 * Thrown if the state of the transaction is such that it disallows the
 * attempted operation. For example, the transaction is committing and
 * a participant that has prepared attempts to resign.
 *
 * Do we want to remove this and replace it with IllegalStateException as
 * is done in the JTA?
 */

public class WrongStateException extends JAXTXException
{

    public WrongStateException ()
    {
	super();
    }

    public WrongStateException (String s)
    {
	super(s);
    }

    public WrongStateException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


