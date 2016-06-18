package javax.jaxtx.model.jta;

import javax.jaxtx.exceptions.ProtocolViolationException;

/**
 * A heuristic hazard has occurred: some participants confirmed, some
 * cancelled and some we just don't know!
 */

public class HeuristicHazardException extends ProtocolViolationException
{

    public HeuristicHazardException ()
    {
	super();
    }

    public HeuristicHazardException (String s)
    {
	super(s);
    }

    public HeuristicHazardException (String s, int errorcode)
    {
	super(s, errorcode);
    }

}
