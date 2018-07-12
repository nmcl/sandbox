import javax.transaction.*;
import javax.transaction.xa.*;

import java.lang.IllegalAccessException;

public class JTAAbort
{
	public void example ()
	{
		try
		{
			javax.transaction.TransactionManager tm = // obtain from JNDI

			if (tm != null)
			{
				System.out.println("Starting top-level transaction.");

				tm.begin();

				javax.transaction.Transaction theTransaction = tm.getTransaction();

				if (theTransaction != null)
				{
					System.out.println("\nRolling back transaction.");

					theTransaction.rollback();

					/*
					 * Have used Transaction to terminate transaction, so it will remain associated
					 * with the thread.
					 */
					 
					System.out.println("\nTransaction now: " + theTransaction);

					/*
					 * Explicitly suspend the aborted transaction.
					 */
					 
					theTransaction = tm.suspend();

					System.out.println("\nSuspended: " + theTransaction);
				}
				else
				{
					System.err.println("Error - could not get transaction!");
					tm.rollback();
				}
			}
			else
				System.err.println("Error - could not get transaction manager!");
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}

}
