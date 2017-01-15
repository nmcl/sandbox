/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CosTranNames.h,v 1.3 1998/06/29 09:35:35 nmcl Exp $
 */

#ifndef COSTRANSACTIONS_COSTRANNAMES_H_
#define COSTRANSACTIONS_COSTRANNAMES_H_

/*
 * Simple renaming macros for portability.
 *
 * Slightly different from C++ version in that we don't need the
 * typedefs. However, we do need some mapping for OTS exceptions which
 * should now be system exceptions.
 */

/*
 * CosTransaction module exceptions
 */

#ifdef ORB_OTS_SUPPORT
#define TransactionRequired TRANSACTION_REQUIRED
#define TransactionRolledBack TRANSACTION_ROLLEDBACK
#define InvalidTransaction INVALID_TRANSACTION

/*
 * We need to check this. WRONG_TRANSACTION is supposed to
 * be a system exception (in the latest draft of the OTS),
 * but two ORBs have it as a user exception.
 */

#ifndef BROKEN_WRONG_TRANSACTION
#define WrongTransaction WRONG_TRANSACTION
#endif

#endif

#define COS_TRAN(name) CosTransactions./**/name

#endif
