/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LocalAAR.h,v 1.1 1993/11/03 14:24:00 nmcl Exp $
 */

#ifndef LOCALAAR_H_
#define LOCALAAR_H_

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ABORT_H_
#  include <StubGen/Abort.h>
#endif

/*
 *
 * Local Atomic Action Record Class
 *
 */

#ifndef CRASHRECR_H_
#  include "CrashRecR.h"
#endif

enum AAStatus
{
    UNRESOLVED_AA, COMMIT_AA, ABORT_AA, COMMITTED_AA, ABORTED_AA
};

class AARecord : public CrashRecRecord
{
public:
    /* Constructor and destructor */

    AARecord(const Uid& actUid);
    ~AARecord();

    ostream& print (ostream& strm) const;
    ostream& id (ostream& strm) const;
    Boolean resolve();

private:
    Uid            AAUid;
    AAStatus       status;
};

#endif // LOCALAAR_H_
