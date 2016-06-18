package javax.jaxtx.model.btp;

/**
 * A qualifier for the core transaction engine to use. Qualifiers in BTP
 * can be used to augment the protocol. For example, when enlisting a
 * participant with a transaction, it is possible to specify a caveat on
 * the enroll via a suitable qualifier, such that the coordinator knows
 * (in this example) that the participant will cancel if it does not hear
 * from the coordinator within 24 hours.
 */

public interface Qualifier
{
    /**
     * @return the unique name for this qualifier.
     */

    public String qualifierName ();

    /**
     * @return the context of this qualifier.
     */

    public String content ();
}
