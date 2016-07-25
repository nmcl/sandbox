/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActionH.h,v 1.8 1995/09/18 15:15:20 ngdp Exp $
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

class ostream;
class Uid;
class Buffer;

//  @NoRemote, @NoMarshall
class ActionInfo;


//  @NoRemote, @NoMarshall	
class ActionHierarchy
{
public:
    /* Constructors and destructor */

    ActionHierarchy (unsigned int);
    ActionHierarchy (const ActionHierarchy&);
    ~ActionHierarchy ();

    friend ostream& operator<< (ostream&, const ActionHierarchy&);

    ActionHierarchy& operator= (const ActionHierarchy&);
    Boolean operator== (const ActionHierarchy&) const;
    
    Boolean add (const Uid&, AtomicAction::ActionType = AtomicAction::TOP_LEVEL);
    Boolean establishHierarchy ();
    void forgetDeepest ();
    
    unsigned int depth () const;
    unsigned int findCommonPrefix (const ActionHierarchy&) const;
    
    const Uid& getDeepestActionUid () const;
    const ActionInfo& getActionInfo (unsigned int) const;

    Boolean isAncestor (const Uid&) const;
    
    Boolean pack (Buffer&) const;
    Boolean unpack (Buffer&);

private:

    ActionInfo *hierarchy;
    unsigned int maxHierarchyDepth;
    unsigned int currentDepth;
};

#endif
