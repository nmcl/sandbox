/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ErrCodes.h,v 1.2 1998/11/20 08:28:28 nmcl Exp $
 */

#ifndef ERRCODES_H_
#define ERRCODES_H_

#ifdef STUB
#  pragma @NoRename
#endif

/*
 *
 * Arjuna System Error Codes
 *
 */

/* Basic RPC codes */
/* Atomic Action codes */

static const int AA_INVALID_ORDER = 1;
static const int AA_NESTING_ERROR = 2;
static const int AA_PREPARE_FAILED = 3;
static const int AA_PHASE2_ERROR = 4;

/* LockManager codes */

static const int LM_BAD_HIERARCHY = 1;
static const int LM_PROPAGATE_FAILED = 2;
static const int LM_LOCKLOAD_FAILED = 3;
static const int LM_ACTIVATE_FAILED = 4;
static const int LM_LOCKUNLOAD_FAILED = 5;
static const int LM_RELEASE_FAILED = 6;
static const int LM_BAD_PARAMS = 7;

/* Buffer management codes */
/* AbstractRecord  codes */

static const int AR_NOT_ALLOWED = 1;
static const int AR_OP_IGNORED = 2;
static const int AR_ADD_FAILED = 3;

/* ObjectStore codes */

static const int OS_EMPTY_STATE = 1;
static const int OS_WRITE_FAILED = 2;
static const int OS_CORRUPT = 3;
static const int OS_TOO_BIG = 4;
static const int OS_REMOVE_FAILED = 5;
static const int OS_COMMIT_FAILED = 6;
static const int LS_BAD_SLOT = 11;
static const int LS_SEGMENT_ALLOC_FAILED = 12;
static const int LS_MAP_ALLOC_FAILED = 13;

/* StateManager codes */

static const int SM_ACTIVATE_FAILED = 1;
static const int SM_DEACTIVATE_FAILED = 2;
static const int SM_DESTROY_FAILED = 3;
static const int SM_MISSING_TERMINATE = 4;
static const int SM_NOTACTIVE = 5;
static const int SM_DELETE_ACTIVE = 6;
static const int SM_RESTORE_FAILED = 7;
static const int SM_SAVE_FAILED = 8;
static const int SM_ACTIONS_ACTIVE = 9;

/* Action RPC codes */
/* Shared memory & Semaphore system codes */

static const int SHM_CREATE_FAILED = 1;
static const int SHM_ATTACH_FAILED = 2;
static const int SHM_SIZE_MISMATCH = 3;
static const int SHM_INVALID = 4;


/* General codes */
/* Crash recovery codes */
/* Naming & Binding codes */
/* Replication system codes */
/* Object Clustering codes */

#endif
