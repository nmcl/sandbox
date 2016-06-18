package javax.jaxtx.model.btp;

/**
 * An instance of this interface is created for each inferior when
 * its status is required.
 */

public interface StatusItem
{

    /**
     * @return the identifier of the inferior this instance represents.
     */

    public String inferiorHandle ();

    /**
     * @return the status of the inferior this instance represents. Should
     * be an value from TwoPhaseStatus.
     *
     * @see javax.jaxtx.model.btp.TwoPhaseStatus
     */

    public int status ();
    
    /**
     * @return the qualifiers.
     */

    public Qualifier[] qualifiers ();
    
}
