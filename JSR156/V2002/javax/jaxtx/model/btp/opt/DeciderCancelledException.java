package javax.jaxtx.model.btp.opt;

import javax.jaxtx.exceptions.JAXTXException;

/**
 * Thrown if all of the inferiors mentioned in prepare or confirm decide
 * to cancel.
 */

public class DeciderCancelledException extends JAXTXException
{

    public DeciderCancelledException ()
    {
	super();
    }

    public DeciderCancelledException (String s)
    {
	super(s);
    }

    public DeciderCancelledException (String s, int errorcode)
    {
	super(s, errorcode);
    }

}
