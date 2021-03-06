package com.arjuna.ats.txoj.logging;

/**
 * The various levels of facility codes that are available.
 * Typically one for each major component in the system.
 *
 * <ul>
 * <li>FAC_CONCURRENCY_CONTROL = 0x0000002 (concurrency control classes).
 * <li>FAC_LOCK_STORE = 0x00010000 (lock store implementations).
 * </ul>
 *
 */
 
public class FacilityCode extends com.arjuna.common.util.logging.FacilityCode
{

public static final long FAC_CONCURRENCY_CONTROL = 0x00000002;
public static final long FAC_LOCK_STORE = 0x00010000;

public long getLevel (String level)
    {
	if (level.equals("FAC_CONCURRENCY_CONTROL"))
	    return FAC_CONCURRENCY_CONTROL;
	if (level.equals("FAC_LOCK_STORE"))
	    return FAC_LOCK_STORE;

	return FacilityCode.FAC_NONE;
    }

    /**
     * @return the string representation of the facility level. Note, this
     * string is intended only for debugging purposes, and cannot be fed
     * back into the debug system to obtain the facility level that it
     * represents.
     *
     * @since JTS 2.1.2.
     */

public String printString (long level)
    {
	if (level == FacilityCode.FAC_ALL)
	    return "FAC_ALL";
	
	if (level == FacilityCode.FAC_NONE)
	    return "FAC_NONE";
	
	String sLevel = null;
	
	if ((level & FAC_CONCURRENCY_CONTROL) != 0)
	    sLevel = ((sLevel == null) ? "FAC_CONCURRENCY_CONTROL" : " & FAC_CONCURRENCY_CONTROL");
	if ((level & FAC_LOCK_STORE) != 0)
	    sLevel = ((sLevel == null) ? "FAC_LOCK_STORE" : " & FAC_LOCK_STORE");

	return ((sLevel == null) ? "FAC_NONE" : sLevel);
    }
		
}

