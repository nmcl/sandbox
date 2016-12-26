/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: LicenceType.java,v 1.1.2.1.14.1.2.2.2.2.2.1.10.1.8.3.54.1 2002/05/21 14:08:12 jcoleman Exp $
 */

package com.arjuna.ArjunaCommon.Core;

/**
 * The various types of licence that are available.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: LicenceType.java,v 1.1.2.1.14.1.2.2.2.2.2.1.10.1.8.3.54.1 2002/05/21 14:08:12 jcoleman Exp $
 * @since JTS 2.1.
 */

public class LicenceType
{

    /*
     * Currently these are all "positive" attributes. Therefore, the lack
     * of something in a licence means it's not available.
     */

public static final int TRANSACTION_CORE = 0x00000001;
public static final int COMMIT_TWO_PHASE = 0x00000002;  // absence forces to one phase only
public static final int TRANSACTIONAL_OBJECTS = 0x00000004;

    /*
     * For limited transactions we use the 2 high-order bytes
     * to specify the number of transactions. If we start to use
     * up these bytes for other licence types, then we should add
     * another licence field (which could be used for other things
     * as well.)
     */

public static final int UNLIMITED_TWO_PHASE_TRANSACTIONS = 0x00000008;

    /**
     * @since JTS 2.1.1.
     */

public static final int UNLIMITED_ONE_PHASE_TRANSACTIONS = 0x00000010;

    /**
     * @since JTS 2.1.1.
     */

public static final int NUMBER_TRANSACTIONS_100 = 0x01000000;
public static final int NUMBER_TRANSACTIONS_1000 = 0x02000000;
public static final int NUMBER_TRANSACTIONS_10000 = 0x040000000;
public static final int NUMBER_TRANSACTIONS_100000 = 0x08000000;

public static final String litype (int li)
    {
	String v = null;
	
	if ((li & LicenceType.TRANSACTION_CORE) != 0)
	    v = "TRANSACTION_CORE";
	
	if ((li & LicenceType.COMMIT_TWO_PHASE) != 0)
	    v += " & COMMIT_TWO_PHASE";
	
	if ((li & LicenceType.TRANSACTIONAL_OBJECTS) != 0)
	    v += " & TRANSACTIONAL_OBJECTS";

	boolean limited = true;
	
	if ((li & LicenceType.UNLIMITED_TWO_PHASE_TRANSACTIONS) != 0)
	{
	    v += " & UNLIMITED_TWO_PHASE_TRANSACTIONS";

	    limited = false;
	}
	else
	    limited = true;

	if ((li & LicenceType.UNLIMITED_ONE_PHASE_TRANSACTIONS) != 0)
	{
	    v += " & UNLIMITED_ONE_PHASE_TRANSACTIONS";
	}
	else
	    limited = true;

	if (limited)
	{
	    if ((li & LicenceType.NUMBER_TRANSACTIONS_100) != 0)
		v += " & NUMBER_TRANSACTIONS_100";

	    if ((li & LicenceType.NUMBER_TRANSACTIONS_1000) != 0)
		v += " & NUMBER_TRANSACTIONS_1000";

	    if ((li & LicenceType.NUMBER_TRANSACTIONS_10000) != 0)
		v += " & NUMBER_TRANSACTIONS_10000";

	    if ((li & LicenceType.NUMBER_TRANSACTIONS_100000) != 0)
		v += " & NUMBER_TRANSACTIONS_100000";
	}

	return v;
    }

public static final String checkAll ()
    {
	String v = null;
	
	if (Lector.confirm(LicenceType.TRANSACTION_CORE))
	    v = "TRANSACTION_CORE";
	
	if (Lector.confirm(LicenceType.COMMIT_TWO_PHASE))
	    v += " & COMMIT_TWO_PHASE";
	
	if (Lector.confirm(LicenceType.TRANSACTIONAL_OBJECTS))
	    v += " & TRANSACTIONAL_OBJECTS";

	boolean limited = true;
	
	if (Lector.confirm(LicenceType.UNLIMITED_TWO_PHASE_TRANSACTIONS))
	{
	    v += " & UNLIMITED_TWO_PHASE_TRANSACTIONS";

	    limited = false;
	}
	else
	    limited = true;

	if (Lector.confirm(LicenceType.UNLIMITED_ONE_PHASE_TRANSACTIONS))
	{
	    v += " & UNLIMITED_ONE_PHASE_TRANSACTIONS";
	}
	else
	    limited = true;

	if (limited)
	{
	    if (Lector.confirm(LicenceType.NUMBER_TRANSACTIONS_100))
		v += " & NUMBER_TRANSACTIONS_100";

	    if (Lector.confirm(LicenceType.NUMBER_TRANSACTIONS_1000))
		v += " & NUMBER_TRANSACTIONS_1000";

	    if (Lector.confirm(LicenceType.NUMBER_TRANSACTIONS_10000))
		v += " & NUMBER_TRANSACTIONS_10000";

	    if (Lector.confirm(LicenceType.NUMBER_TRANSACTIONS_100000))
		v += " & NUMBER_TRANSACTIONS_100000";
	}

	return v;
    }
    
}
