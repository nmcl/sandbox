package javax.jaxtx.model.btp.opt;

import javax.jaxtx.exceptions.JAXTXException;

/**
 * Thrown if all of the inferiors mentioned in prepare or cancel decide
 * to confirm.
 */

public class DeciderConfirmedException extends JAXTXException
{

    public DeciderConfirmedException ()
    {
	super();
    }

    public DeciderConfirmedException (String s)
    {
	super(s);
    }

    public DeciderConfirmedException (String s, int errorcode)
    {
	super(s, errorcode);
    }

}
