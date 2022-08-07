/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: CosTransactionsNames.java,v 1.1.10.1.24.1 2000/12/21 11:21:12 nmcl Exp $
 */

package com.arjuna.CosTransactions;

import com.arjuna.ArjunaCommon.Common.ClassName;

/*
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: CosTransactionsNames.java,v 1.1.10.1.24.1 2000/12/21 11:21:12 nmcl Exp $
 * @since JTS 1.0.
 */

public class CosTransactionsNames
{

public static ClassName AbstractRecord_OTS_ResourceRecord ()
    {
	return new ClassName("RecordType.OTS_RECORD");
    }
    
public static ClassName AbstractRecord_OTS_AbstractRecord ()
    {
	return new ClassName("RecordType.OTS_ABSTRACTRECORD");
    }

};
