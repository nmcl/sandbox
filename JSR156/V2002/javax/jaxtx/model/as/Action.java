package javax.jaxtx.extensions.as;

import javax.jaxtx.Outcome;

public interface Action
{

    public Outcome processSignal (org.w3c.dom.Document signal) throws ActionFaultException, javax.jaxtx.exceptions.SystemException;

}
