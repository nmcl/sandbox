package javax.jaxtx.model.btp.opt;

import javax.jaxtx.exceptions.JAXTXException;

/**
 * Thrown if an inferior confirms rather than cancelling or preparing.
 */

public class InferiorConfirmedException extends JAXTXException
{

    public InferiorConfirmedException ()
    {
	super();
    }

    public InferiorConfirmedException (String s)
    {
	super(s);
    }

    public InferiorConfirmedException (String s, int errorcode)
    {
	super(s, errorcode);
    }

}
