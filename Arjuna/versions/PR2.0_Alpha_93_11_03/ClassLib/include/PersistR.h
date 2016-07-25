/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PersistR.h,v 1.1 1993/11/03 12:29:05 nmcl Exp $
 */

#ifndef PERSISTR_H_
#define PERSISTR_H_

/*
 *
 * Persistence Record Class
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef RECORDT_H_
#  include <Arjuna/RecordT.h>
#endif

#ifndef RECOVERYR_H_
#  include "RecoveryR.h"
#endif

class ObjectState;
class ObjectStore;
class StateManager;
class Uid;

class PersistenceRecord : public RecoveryRecord
{
public:
    /* Constructors and destructor */

    PersistenceRecord(ObjectState *os, ObjectStore *objStore, StateManager *sm);
    PersistenceRecord(); // used by crash recovery

    virtual ~PersistenceRecord ();

    /* inherited member functions */

    virtual RecordType type_is () const;

    virtual void top_level_abort ();
    virtual void top_level_commit ();
    virtual PrepareOutcome top_level_prepare ();

    virtual Boolean restore_state (ObjectState& os, ObjectType ot);
    virtual Boolean save_state (ObjectState& os, ObjectType ot);

    virtual ostream& print (ostream & strm) const;
    virtual const TypeName type () const;

private:
    Boolean shadowMade;
    ObjectStore *store;
};

#endif
