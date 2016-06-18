package javax.jaxtx.model.btp.opt;

import javax.jaxtx.exceptions.JAXTXException;

/**
 * Thrown if the inferior identifier is invalid within the
 * scope it has been used.
 */

public class InvalidInferiorException extends JAXTXException
{

    public InvalidInferiorException ()
    {
	super();
    }

    public InvalidInferiorException (String s)
    {
	super(s);
    }

    public InvalidInferiorException (String s, int errorcode)
    {
	super(s);
    }

}
