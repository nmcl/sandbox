/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: Param.h,v 1.2 1994/10/28 12:39:04 ngdp Exp $
 */

#ifndef PARAM_H_
#define PARAM_H_

#include <System/sys/types.h>

//@NoRemote, @NoMarshall
class RpcBuffer;

//@NoRemote, @UserMarshall
class Param
{
public:
    Param ();
    Param (size_t);
    ~Param();
    
    void marshall (RpcBuffer&) const;
    void unmarshall (RpcBuffer&);
private:
    void *p;
    size_t size;
};

#endif
