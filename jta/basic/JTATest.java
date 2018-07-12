import javax.transaction.*;
import javax.transaction.xa.*;

import java.lang.IllegalAccessException;

/*
 * JTA supports a TransactionManager API, which is the server-side handle on the TM. It has more operations
 * than the UserTransaction API, which is really intended for the client-side. Both UserTransaction and
 * TransactionManager are similar to the OTS Current pseudo-object.
 */

public class JTATest
{
	public void example ()
	{
		XAResource xares = // get XAResource from RM, JCA, JMS etc.
		
		/*
		 * We should have a reference to a factory object (see JTA
		 * specification). However, for simplicity we will ignore this.
		 */

		try
		{
			/*
			 * Get the transaction reference from JNDI.
			 */
			 
			javax.transaction.TransactionManager tm = new InitialContext().lookup("java:comp/TransactionManager");
			
			if (tm != null)
			{
				System.out.println("Starting top-level transaction.");

				/*
				 * Start the transaction. tm will do the thread-to-transaciton association.
				 */
				 
				tm.begin();

				/*
				 * Each transaction is represented by a Transaction instance. We can commit or rollback on
				 * this instance, but it won't do thread-to-transaction work, i.e., this is essentially
				 * the same as the OTS Coordinator and Terminator interfaces.
				 */
				 
				javax.transaction.Transaction theTransaction = tm.getTransaction();

				if (theTransaction != null)
				{
					System.out.println("\nTrying to register resource with transaction.");

					/*
					 * Only the Transaction has the method to register participants.
					 */
					 
					if (!theTransaction.enlistResource(theResource))
					{
						System.err.println("Error - could not enlist resource in transaction!");
						tm.rollback();
						
						return;
					}
					else
						System.out.println("\nResource enlisted successfully.");
					
					/*
					 * Do some work and decide whether to commit or rollback.
					 * (Assume commit for example.)
					 */

					Synchronization s = // some synchronization instance.
					
					tm.getTransaction().registerSynchronization(s);

					System.out.println("\nCommitting transaction.");

					if (tmCommit)
						System.out.println("Using transaction manager.\n");
					else
						System.out.println("Using transaction.\n");

					if (tmCommit)
						tm.commit();
					else
						tm.getTransaction().commit();
					}
				}
				else
				{
					System.err.println("Error - could not get transaction!");
					tm.rollback();

					return;
				}

				System.out.println("\nTest completed successfully.");
			}
			else
			{
				System.err.println("Error - could not get transaction manager!");
			}
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}

}
