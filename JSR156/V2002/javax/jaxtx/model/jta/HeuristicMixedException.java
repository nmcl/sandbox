package javax.jaxtx.model.jta;

import javax.jaxtx.exceptions.ProtocolViolationException;

/**
 * A heuristic mixed has occurred: some participants confirmed and some
 * cancelled. Non-atomic outcomes.
 */

public class HeuristicMixedException extends ProtocolViolationException
{

    public HeuristicMixedException ()
    {
	super();
    }

    public HeuristicMixedException (String s)
    {
	super(s);
    }

    public HeuristicMixedException (String s, int errorcode)
    {
	super(s, errorcode);
    }

}
