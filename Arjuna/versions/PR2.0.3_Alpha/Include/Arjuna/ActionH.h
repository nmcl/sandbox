/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActionH.h,v 1.2 1993/06/11 13:09:58 nmcl Exp $
 */

#ifndef ACTIONH_H_
#define ACTIONH_H_

#ifdef STUB
#  pragma @NoRename
#endif

/*
 *
 * Atomic Action Hierarchy class
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

class Uid;
class Buffer;

//  @NoRemote, @NoMarshal
struct ActionInfo;          // xlC bug or ANSI bug??


//  @NoRemote, @NoMarshall	
class ActionHierarchy
{
public:
    /* Constructors and destructor */

    ActionHierarchy (int);
    ~ActionHierarchy ();

    void add (const Uid&, AtomicAction::ActionType);

    Boolean establishHierarchy ();

    const Uid& getUid (int) const;
    const AtomicAction::ActionType getActionType (int) const;
    
    Boolean pack (Buffer&) const;
    Boolean unpack (Buffer&);
    
private:
    int findCommonPrefix (const ActionHierarchy&) const;

    ActionInfo *hierarchy;
    int maxHierarchyDepth;
    int currentDepth;
    
};

#endif
