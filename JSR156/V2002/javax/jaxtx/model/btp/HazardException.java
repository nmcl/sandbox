package javax.jaxtx.model.btp;

import javax.jaxtx.exceptions.ProtocolViolationException;

/**
 * A heuristic hazard has occurred: some participants confirmed, some
 * cancelled and some we just don't know!
 */

public class HazardException extends ProtocolViolationException
{

    public HazardException ()
    {
	super();
    }

    public HazardException (String s)
    {
	super(s);
    }

    public HazardException (String s, int errorcode)
    {
	super(s, errorcode);
    }

}
