/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: COControl.h,v 1.12 1994/07/20 13:40:44 nmcl Exp $
 */

#ifndef COCONTROL_H_
#define COCONTROL_H_

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

#ifndef GROUPDATA_H_
#  include <ArjServers/GroupData.h>
#endif

#ifndef REPDESC_H_
#  include <ArjServers/RepDesc_stub.h>
#endif

#ifndef REMOTEOSTYPES_H_
#  include <Replication/RemoteOSTypes.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef STATEMAN_H_
#  include <Arjuna/StateMan.h>
#endif


// @NoRemote, @NoMarshall
class TemplateObject;

// @NoRemote, @NoMarshall
class AtomicAction;


//@NoRemote, @NoMarshall
enum TemplateType { Normal, WildCard, GlobalWildCard };

//@NoRemote, @NoMarshall
enum ActionType { NormalAction, TopLevel };


//@NoRemote, @NoMarshall
class ComplexObjectControl
{
public:
    ComplexObjectControl (ActionType = NormalAction);
    virtual ~ComplexObjectControl ();

    /* Template manipulation routines. */

    /*
     * The following routines only work if a template either
     * does not exist (add) or does exist (delete). To be able
     * to add/delete, no replicas can be mentioned at the db.
     * Therefore, no consistency problems arise from this.
     */

    OutCome addTemplate (const Uid& groupUid, char** locations, int number);
    OutCome deleteTemplate (const Uid& toDelete);

    /*
     * The following routines work on already existing templates.
     * As such, we must guarantee that such changes keep the replica
     * group consistent.
     */

    OutCome addToTemplate (const Uid& templateUid, char* location);
    OutCome deleteFromTemplate (const Uid& templateUid, char* location);

    /*
     * Complex object manipulation routines.
     * The Boolean parameter indicates whether this is a new group or not.
     */

    OutCome groupWithTemplate (const Uid& templateUid, const StateManager& toGroup,
			       Boolean isNew = FALSE);
    OutCome ungroupFromTemplate (const Uid& templateUid, const StateManager& toUngroup);

    /* Replica manipulation routines. */

    OutCome associateGroups (const StateManager& assignTo,
			     const StateManager& toAssign);
    OutCome disassociateGroups (const StateManager& assignedTo,
				const StateManager& assigned);

protected:
    /*
     * The following are various routines for template/composite object
     * manipulation which are not of immediate use to typical application
     * programmers.
     */

    OutCome addTemplate (const Uid& groupUid, TemplateType t,
			 char** locations, int number);
    OutCome addTemplate (const Uid& groupUid, GroupData& toAdd,
			 TemplateType t);

    OutCome replaceTemplate (const Uid& groupUid,
			     char** locations, int number);
    OutCome replaceTemplate (const Uid& groupUid, TemplateType t,
			     char** locations, int number);
    OutCome replaceTemplate (const Uid& groupUid, GroupData& toReplace,
			     TemplateType t);

    OutCome getTemplate (const Uid& groupUid, GroupData*& toGet,
			 LockMode lmode = READ);

    OutCome addReplicas (const Uid& groupUid, const ReplicaDescriptor& toAdd);
    OutCome deleteReplicas (const Uid& groupUid, const ReplicaDescriptor& toDelete);

    OutCome addTemplateReplicas (const Uid& groupUid, const ReplicaDescriptor& toAdd, Boolean isNew = FALSE);
    OutCome deleteTemplateReplicas (const Uid& groupUid, const ReplicaDescriptor& toDelete, Boolean isNew = FALSE);

    OutCome associateGroups (const Uid& assignTo, const Uid& toAssign);
    OutCome associateGroups (const Uid& assignTo, GroupData& toAssign);
    OutCome disassociateGroups (const Uid& assignedTo, const Uid& assigned);
    OutCome disassociateGroups (const Uid& assignedTo, GroupData& assigned);

private:
    Boolean setupDatabase ();
    void createStructure (const Uid&, GroupData*&, char**, int) const;

    void setTemplateType (GroupData&, TemplateType) const;
    OutCome manipulateTemplateGroup (const Uid&, char*, Boolean);
    OutCome modifyTemplateReplicas (const Uid&, const ReplicaDescriptor&, Boolean, Boolean);

    Boolean makeConsistent (const Uid&, const Uid&);
    GroupData* constructDifference (const TemplateObject&, const ReplicaDescriptor&, const Uid&, ReplicaDescriptor*&);
    
    void beginAction ();
    Boolean endAction (OutCome);

    NSInterface*  GV;
    AtomicAction* A;
    ActionType    actionType;
    const char*   rootLocation;
};


#include <Replication/COControl.n>

#endif
