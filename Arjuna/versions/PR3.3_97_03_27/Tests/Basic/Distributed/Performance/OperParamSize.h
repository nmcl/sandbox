/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: OperParamSize.h,v 1.2 1994/01/06 12:23:09 ngdp Exp $
 */

#ifndef OPERPARAMSIZE_H_
#define OPERPARAMSIZE_H_

#include "Param.h"

class RpcBuffer;

class OperParamSize
{
public:
    OperParamSize();
    ~OperParamSize();

    void nullOp (Param&) const;
};

#endif
