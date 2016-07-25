/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PersistR.h,v 1.3 1993/12/14 11:28:53 nmcl Exp $
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
#  include "RecovryR.h"
#endif


class ObjectState;
class ObjectStore;
class StateManager;
class Uid;

class PersistenceRecord : public RecoveryRecord
{
public:
    /* Constructors and destructor */

    PersistenceRecord(StateManager* sm);

    virtual ~PersistenceRecord ();

    void setStore(ObjectStore*);
    void setState (ObjectState*);

    /* inherited member functions */

    virtual RecordType typeIs () const;

    virtual Boolean topLevelAbort ();
    virtual Boolean topLevelCommit ();
    virtual PrepareOutcome topLevelPrepare ();

    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual Boolean save_state (ObjectState&, ObjectType);

    virtual ostream& print (ostream&) const;
    virtual const TypeName type () const;

    Boolean doSave () const;

protected:
    PersistenceRecord(); // used by crash recovery

private:
    void setStore (char*);
	Boolean packStore (ObjectState&) const;

    Boolean shadowMade;
    ObjectStore *store;
};

#endif
