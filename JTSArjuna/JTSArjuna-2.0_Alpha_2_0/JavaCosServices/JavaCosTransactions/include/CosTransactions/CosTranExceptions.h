/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CosTranExceptions.h,v 1.3 1998/07/06 13:29:23 nmcl Exp $
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
import NAMESPACE_(CORBA).WrongTransaction;
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
