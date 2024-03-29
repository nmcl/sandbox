/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FacCode.h,v 1.1 1994/10/10 10:26:50 ngdp Exp $
 */

#ifndef FACCODE_H_

#ifndef STUB
#  include <Common/FacCode_stub.h>
#else

#define FACCODE_H_
/* 
 * Enumerated types for FacilityCodes
 */
 
enum FacilityCode
{
    FAC_BASIC_RPC = 0x0001,
    FAC_ATOMIC_ACTION = 0x0002,
    FAC_CONCURRENCY_CONTROL = 0x0004,
    FAC_BUFFER_MAN = 0x0008,
    FAC_ABSTRACT_REC = 0x0010,
    FAC_OBJECT_STORE = 0x0020,
    FAC_STATE_MAN = 0x0040,
    FAC_ACTION_RPC = 0x0080,
    FAC_SHMEM = 0x0100,
    FAC_GENERAL = 0x0200,
    FAC_CRASH_RECOVERY = 0x0400,
    FAC_NAMING_AND_BINDING = 0x0800,
    FAC_REPLICATION = 0x1000,
    FAC_OBJECT_CLUSTERING = 0x2000,
    FAC_USER1 = 0x4000,
    FAC_USER2 = 0x8000,
    FAC_ALL = 0xffff
};

#endif
#endif
