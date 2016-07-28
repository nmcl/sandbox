/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SingleTypeMS.h,v 1.4 1995/09/19 10:24:33 ngdp Exp $
 */

#ifndef SINGLETYPEMS_H_
#define SINGLETYPEMS_H_

#ifndef SYS_TYPES_H_
#  include <os/sys/types.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef FSSTORE_H_
#  include <ObjectStore/FSStore.h>
#endif

#ifndef MAPPEDFILE_H_
#  include <ObjectStore/MappedFile.h>
#endif

/*
 * Change the following constants at compile time if you require more/less states
 */

const unsigned int MAX_STATES = 2048;	/* number of objects per store */
const unsigned int MAX_STORES = 7;	/* number of stores */

// @NoRemote, @NoMarshall
class ProcessMutex;

// @NoRemote, @NoMarshall
class SingleStoreData;

// @NoRemote, @NoMarshall
class SingleTypeMappedStore : public FileSystemStore
{
public:
    SingleTypeMappedStore (const char *, const TypeName, Boolean = TRUE);
    virtual ~SingleTypeMappedStore ();

    virtual Boolean allObjUids (const TypeName, ObjectState&,
				ObjectStore::StateStatus);
    
    virtual Boolean allTypes (ObjectState&) const;
    virtual ObjectStore::StateStatus currentState (const Uid&, const TypeName);
    
    virtual Boolean hide_state (const Uid&, const TypeName);
    virtual Boolean reveal_state (const Uid&, const TypeName);
    
    virtual Boolean commit_state (const Uid&, const TypeName);

    const TypeName type () const;
    
protected:
    virtual Boolean allTypes (ObjectState&, const char *) const;
    
    virtual char *genPathName (const Uid&, const TypeName,
			       ObjectStore::StateType) const;    

    virtual Boolean supressEntry (const char *) const;
    virtual Boolean lock (int, int) const;
    virtual Boolean unlock (int) const;

 
private:
    ObjectState* read_state (const Uid&, const TypeName, ObjectStore::StateType);
    Boolean remove_state (const Uid&, const TypeName, ObjectStore::StateType);
    Boolean write_state (const Uid&, const TypeName, const ObjectState&,
			 ObjectStore::StateType);


    Boolean lock ();
    Boolean unlock ();

    void rename (const Uid&);

    Boolean readStates ();
    Boolean writeStates ();

    void deleteList ();
    void removeElement (SingleStoreData*&, SingleStoreData*&);

    Boolean findOldState (SingleStoreData*&, const Uid&, Boolean, const ObjectState&);
    Boolean findMemory (SingleStoreData*&, const ObjectState&, off_t&);

    off_t dataSegment ();
    off_t headerSize ();
    Boolean sizeUp (off_t);
    
    /* NOTE: only operate on one underlying store at a time so these are not arrays */

    TypeName typeStored;
    SingleStoreData *head;
    ProcessMutex *mutex;
    MappedFile *currentStore;
    unsigned int stateCount;
    
    MappedFile *mappedFiles[MAX_STORES];
    
    Boolean writeShadow;
};

#include <ObjectStore/SingleTypeMS.n>

#endif
