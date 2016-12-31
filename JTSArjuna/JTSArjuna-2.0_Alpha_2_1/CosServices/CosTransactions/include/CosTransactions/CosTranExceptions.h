/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CosTranExceptions.h,v 1.1.2.1 2000/05/02 08:41:13 nmcl Exp $
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
import NAMESPACE_(CORBA).WrongTransaction;
#else
import org.omg.CosTransactions.WrongTransaction;
#endif
import NAMESPACE_(CORBA).INVALID_TRANSACTION;
import NAMESPACE_(CORBA).TRANSACTION_REQUIRED;
import NAMESPACE_(CORBA).TRANSACTION_ROLLEDBACK;
#else
import org.omg.CosTransactions.WrongTransaction;
import org.omg.CosTransactions.InvalidTransaction;
import org.omg.CosTransactions.TransactionRequired;
import org.omg.CosTransactions.TransactionRolledBack;
#endif

#endif
