/*
 * Copyright (C) 1996
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

#ifndef COMMON_KUBERA_H_
#define COMMON_KUBERA_H_

enum ObjectType
{
    RECOVERABLE, ANDPERSISTENT, NEITHER
};

enum ObjectStatus
{
    CONNECTED, DISCONNECTED, DELETED
};

extern const char* _separator;

#endif
