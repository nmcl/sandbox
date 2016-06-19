/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef SINGLECOPY_H_
#define SINGLECOPY_H_

#ifndef RANDOM_H_
#include <Random.h>
#endif

#ifndef COMMON_H_
#include <common.h>
#endif

#ifndef PROTOCOL_H_
#include "Protocol.h"
#endif

/*
 * must read & write only to primary
 */

class SingleCopy : public Protocol
{
public:
    SingleCopy (long, long, double);
    virtual ~SingleCopy ();
    
    void SetWriteQuorum (long);
    void SetReadQuorum (long);
    long GetMinimum ();

    virtual Opcode PerformOperation (lock_code, Client*);
    virtual boolean CheckQuorum (lock_code, Client*);

    void electPrimary ();
    
private:
    Opcode unlockGroup (Client*);
    Opcode lockGroup (Client*, lock_code);
    boolean checkGroup (Client*, lock_code);

    long primaryCopy;
    UniformStream* US;
};

#endif

