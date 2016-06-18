package javax.jaxtx.model.btp.opt;

import javax.jaxtx.exceptions.JAXTXException;

/**
 * Thrown if the superior identifier is invalid within the
 * scope it has been used.
 */

public class InvalidSuperiorException extends JAXTXException
{

    public InvalidSuperiorException ()
    {
	super();
    }

    public InvalidSuperiorException (String s)
    {
	super(s);
    }

    public InvalidSuperiorException (String s, int errorcode)
    {
	super(s, errorcode);
    }

}
