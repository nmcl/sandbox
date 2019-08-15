/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosTranExceptions.h,v 1.1.2.1.28.1.14.1.42.1 2001/08/09 16:15:35 delsworthy Exp $
 */

#ifndef COSTRANSACTIONS_COSTRANEXCEPTIONS_H_
#define COSTRANSACTIONS_COSTRANEXCEPTIONS_H_

#ifndef COSTRANSACTIONS_COSTRANNAMES_H_
#  include <CosTransactions/CosTranNames.h>
#endif

/*
 * An attempt at a portable way of including the OTS exceptions,
 * which are either mapped as system exceptions or user exceptions,
 * depending upon the Orb.
 */

#ifdef ORB_OTS_SUPPORT
#ifndef BROKEN_WRONG_TRANSACTION
import org.omg.CORBA.WrongTransaction;
#else
import org.omg.CosTransactions.WrongTransaction;
#endif
import org.omg.CORBA.INVALID_TRANSACTION;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CORBA.TRANSACTION_ROLLEDBACK;
#else
import org.omg.CosTransactions.WrongTransaction;
import org.omg.CosTransactions.InvalidTransaction;
import org.omg.CosTransactions.TransactionRequired;
import org.omg.CosTransactions.TransactionRolledBack;
#endif

#endif
