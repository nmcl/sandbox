/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverR.h,v 1.1 1993/11/03 12:28:49 nmcl Exp $
 */

#ifndef CADAVERR_H_
#define CADAVERR_H_

/*
 *
 * Cadaver Record Class
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef RECORDT_H_
#  include <Arjuna/RecordT.h>
#endif

#ifndef PERSISTR_H_
#  include "PersistR.h"
#endif

class ObjectState;
class ObjectStore;
class StateManager;
class Uid;
class ostream;

class CadaverRecord : public PersistenceRecord
{
public:
    /* Constructors & destructor */

    CadaverRecord (ObjectState *os, ObjectStore *objStore, 
		   const char *storeLoc, StateManager *sm);
    ~CadaverRecord ();

    /* inherited public virtual functions from PersistenceRecord */

    virtual void merge (const AbstractRecord *ar);
    virtual Boolean should_merge (const AbstractRecord *ar) const;
    virtual Boolean should_replace (const AbstractRecord *ar) const;
    virtual RecordType type_is () const;

    virtual void nested_abort ();
    virtual void top_level_abort ();
    virtual void top_level_commit ();
    virtual PrepareOutcome top_level_prepare ();

    virtual ostream& print (ostream& strm) const;
    virtual const TypeName type () const;

private:
    ObjectState *oldState;
    RecordType oType;
    char *storeLocation;
    TypeName storeUsed;
    ObjectStore* osUsed;
};

#endif




























