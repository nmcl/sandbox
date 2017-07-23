/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef PASSIVE_H_
#define PASSIVE_H_

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
 * can read from any copy, but must write to primary only
 */

class Passive : public Protocol
{
public:
    Passive (long, long, double);
    virtual ~Passive ();
    
    void SetWriteQuorum (long);
    void SetReadQuorum (long);
    long GetMinimum ();

    virtual Opcode PerformOperation (lock_code, Client*);
    virtual boolean CheckQuorum (lock_code, Client*);

    void electPrimary ();
    
private:
    Opcode unlockGroup (Client*);
    Opcode lockGroup (Client*, lock_code, long);
    boolean checkGroup (Client*, long, lock_code);

    long primaryCopy;
    UniformStream* US;
};

#endif

