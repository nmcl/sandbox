/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StateDict.h,v 1.1 1994/11/29 13:43:42 ngdp Exp $
 */

#ifndef STATEDICT_H_
#define STATEDICT_H_

#ifndef SYS_TYPES_H_
#   include <os/sys/types.h>
#endif

/* 
 * This structure sits in the 1st block of the file containing the states.
 */

struct StateDictionary
{
    StateDictionary();
    void decode ();
    void encode ();
    
    char magic[4];
    off_t originalOffset;
    size_t originalSize;
    off_t shadowOffset;
    size_t shadowSize;
    unsigned short stateHidden;
    unsigned short shadowExists;
};

#endif
