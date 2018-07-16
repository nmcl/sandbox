import javax.transaction.Transaction;
import javax.transaction.xa.XAResource;

public class SimpleTest
{
    public SimpleTest ()
    {
        try
        {
            javax.transaction.TransactionManager transactionManager = // get TM from JNDI

            transactionManager.begin();

            Transaction currentTrans = transactionManager.getTransaction();

            TestResource res1, res2;
            currentTrans.enlistResource( res1 = new TestResource() );
            currentTrans.enlistResource( res2 = new TestResource() );

			/*
			 * When we are finished with the resources we are supposed to delist them, which
			 * is the equivalent of xa_end. We can call deslistResource any time, and can
			 * use XAResource.TMSUSPEND if we intend to come back to it before the transaction
			 * terminates. Typically the application server will do this. For simplicity, some
			 * JTA implementations doesn't require deslistResource to be called by the application
			 * or application server: they'll do it under the covers.
			 */
			 
            currentTrans.delistResource( res2, XAResource.TMSUCCESS );
            currentTrans.delistResource( res1, XAResource.TMSUCCESS );

            transactionManager.commit();
        }
        catch (Exception e)
        {
            e.printStackTrace(System.err);
            System.err.println("ERROR - "+e);
        }
    }

}
