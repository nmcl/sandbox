package com.arjuna.ats.internal.arjuna.common;

import com.hp.mwlabs.lic.*;
import com.hp.mwlabs.lic.exceptions.LicenseError;

public class ArjunaLicenceControl extends LicControl
{

	/* Licence manager string */
	protected static final String LIC_MGR = "ArjunaCore_LicenceManager";
	/* Licence key string */
	protected static final String LIC_KEY = "ArjunaCore_LicenceKey";
	/* Masks for licence parts */
	protected static final int NETMASK = 0xaef4108d;
	protected static final int HOSTMASK = 0x2cab180e;
	protected static final int DATEMASK = 0x84ad1c5f;
	protected static final int CODEMASK = 0xfa68a01d;

	public ArjunaLicenceControl()
	{

		super(LIC_KEY, NETMASK, HOSTMASK, DATEMASK, CODEMASK);
	}

	public String toString()
	{
		return(LIC_MGR);
	}

	public String getKeyID()
	{
		return(LIC_KEY);
	}

        public static String product ()
        {
	    return "ArjunaCore";
	}

	public int generateCode(String[] args)
	{
		int number = 100;	// default number of restricted transactions
		boolean noTimeout = false;
		boolean checkValues = false;
		boolean onePhaseRestriction = false;
		boolean restrictedTwoTransactions = false;
		boolean restrictedOneTransactions = false;
		boolean enableAll = false;
		int code = com.arjuna.ats.internal.arjuna.common.LicenceType.TRANSACTION_CORE;
		for (int i = 0; i < args.length; i++)
		{
			if (args[i].compareTo("-number") == 0)
			{
				if (i == (args.length - 1))
					throw new LicenseError("number parameter with no argument");

				try
				{
					Long m = Long.valueOf(args[i+1]);
					number = m.intValue();
				}
				catch (NumberFormatException e)
				{
					throw new LicenseError(e.getMessage());
				}
				i++;
			}
			else if (args[i].compareTo("-enable") == 0)
			{
				if (i == (args.length - 1))
					throw new LicenseError("enable parameter with no argument");
				if (args[i+1].equals("all"))
				/*
			 	* Enable all functionality.
			 	* 
			 	* At present this is just the same as enabling
			 	* transactional objects, but may not be in future.
			 	*/
	
					enableAll = true;
				else if (args[i+1].equals("commit_one_phase"))
					onePhaseRestriction = true;
				else if (args[i+1].equals("transactional_objects"))
					code = code | com.arjuna.ats.internal.arjuna.common.LicenceType.TRANSACTIONAL_OBJECTS;
				else if (args[i+1].equals("restricted_two_phase"))
					restrictedTwoTransactions = true;
				else if (args[i+1].equals("restricted_one_phase"))
					restrictedOneTransactions = true;
				else
				{
					try
					{
						String hexStart = "0x";
						String hexCode = args[i+1];
	
						if (args[i+1].startsWith(hexCode))
							hexCode = args[i+1].substring(hexCode.length());
	
						Long m = Long.valueOf(hexCode, 16);
						code = code | m.intValue();
					}
					catch (NumberFormatException e)
					{
						throw new LicenseError("enable code is illegal: "+args[i+1]);
					}
				}
				i++;
			}
			else
			{
				throw new LicenseError("unknown argument: " + args[i]);
			}
		}	/* args.length */
		if (enableAll)
			code = code | com.arjuna.ats.internal.arjuna.common.LicenceType.TRANSACTIONAL_OBJECTS;
	
		if (!onePhaseRestriction)
			code = code | com.arjuna.ats.internal.arjuna.common.LicenceType.COMMIT_TWO_PHASE;

		boolean restricted = false;
	
		if (!restrictedTwoTransactions)
			code = code | com.arjuna.ats.internal.arjuna.common.LicenceType.UNLIMITED_TWO_PHASE_TRANSACTIONS;
		else
			restricted = true;

		if (!restrictedOneTransactions)
			code = code | com.arjuna.ats.internal.arjuna.common.LicenceType.UNLIMITED_ONE_PHASE_TRANSACTIONS;
		else
			restricted = true;
	
		if (restricted)
		{
			switch (number)
			{
				case 100:
					code = code | com.arjuna.ats.internal.arjuna.common.LicenceType.NUMBER_TRANSACTIONS_100;
					break;
				case 1000:
					code = code | com.arjuna.ats.internal.arjuna.common.LicenceType.NUMBER_TRANSACTIONS_1000;
					break;
				case 10000:
					code = code | com.arjuna.ats.internal.arjuna.common.LicenceType.NUMBER_TRANSACTIONS_10000;
					break;
				case 100000:
					code = code | com.arjuna.ats.internal.arjuna.common.LicenceType.NUMBER_TRANSACTIONS_100000;
					break;
				default:
					throw new LicenseError("valid numbers are: 100, 1000, 10000, 100000");
			}
		}
		return(code);

	}

	public String getCapabilities()
	{
		com.arjuna.ats.internal.arjuna.common.LicenceType licType;

		licType = new com.arjuna.ats.internal.arjuna.common.LicenceType();
		return(licType.checkAll());
	}

	public String getUsage()
	{
		return("-enable <all|commit_one_phase|transactional_objects|restricted_two_phase|restricted_one_phase> [-number <100|1000|10000|100000>]");
	}

}
