/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 *    This header file contains the definitions of the opcode
 *    and error codes used across the ANSA Action Interface.
 */

/*
 *  $Id: ANSAActOp.h,v 1.1 1993/11/03 14:39:01 nmcl Exp $
 */

// Opcodes

#define AAO_NESTEDABORT     1
#define AAO_NESTEDCOMMIT    2
#define AAO_NESTEDPREPARE   3
#define AAO_TOPLEVELABORT   4
#define AAO_TOPLEVELCOMMIT  5
#define AAO_TOPLEVELPREPARE 6

#define AAO_DONE	    7
#define AAO_NOTDONE         8
