/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosTranNames.h,v 1.1.42.1.42.1 2001/08/09 16:15:35 delsworthy Exp $
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
