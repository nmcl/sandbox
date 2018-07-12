import javax.transaction.xa.*;

public class JTAOrder
{

	public static void main (String[] args)
	{
		try
		{
			XAResource theResource = new TestResource();
			FirstXAResource first = new FirstXAResource();
			LastXAResource last = new LastXAResource();

			javax.transaction.TransactionManager tm = // get TM from JNDI
			
			System.out.println("Starting top-level transaction.");

			tm.begin();

			javax.transaction.Transaction theTransaction = tm.getTransaction();

			/*
			 * This order is not guaranteed by JTA.
			 */
			 
			theTransaction.enlistResource(theResource);
			theTransaction.enlistResource(last);
			theTransaction.enlistResource(first);

			System.err.println("Committing transaction.");

			tm.commit();
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}
}
