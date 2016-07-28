/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: COControl.cc,v 1.22 1994/10/14 10:59:00 nmcl Exp $
 */


#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef TEMPLATEOBJECT_H_
#  include <ArjServers/TemplateObject.h>
#endif

#ifndef COCONTROL_H_
#  include <Replication/COControl.h>
#endif

#ifndef REPLICACREATE_H_
#  include <Replication/ReplicaCreate.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif


extern const char* templateName;
extern const char* wildCardName;
extern const char* globalWildCardName;


ComplexObjectControl::ComplexObjectControl (ActionType toUse)
                                                            : GV(0),
							      A(0),
							      actionType(toUse),
							      rootLocation(0)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "ComplexObjectControl::ComplexObjectControl ()" << endl;
#endif
}

ComplexObjectControl::~ComplexObjectControl ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "ComplexObjectControl::~ComplexObjectControl ()" << endl;
#endif

    if (GV)
        delete GV;
    if (A)
    {
	error_stream << "ComplexObjectControl : Action still running in destructor - will abort." << endl;
	A->Abort();
	delete A;
    }
}

Boolean ComplexObjectControl::setupDatabase ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "Boolean ComplexObjectControl::setupDatabase ()" << endl;
#endif

    int res = -1;

    if (GV)
        return TRUE;

    GV = new NSInterface(res);
    if (res != 0)
    {
        delete GV;
	GV = (NSInterface*) 0;
	return FALSE;
    }

    return TRUE;
}

void ComplexObjectControl::beginAction ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "void ComplexObjectControl::beginAction ()" << endl;
#endif

    if (A != (AtomicAction*) 0)
    {
	error_stream << WARNING << "ComplexObjectControl : Action still running. Will abort first" << endl;
	A->Abort();
	delete A;
    }

    if (actionType == NormalAction)
	A = new AtomicAction;
    else
	A = new TopLevelAction;

    A->Begin();
}

Boolean ComplexObjectControl::endAction (OutCome result)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "Boolean ComplexObjectControl::endAction ( " << result << " )" << endl;
#endif

    Boolean status = FALSE;

    if (!A)
    {
        error_stream << WARNING << "ComplexObjectControl : End called on non-running action. Will ignore" << endl;
	return TRUE;
    }

    if (result == DONE)
        status = (Boolean) (A->End() == COMMITTED);
    else
        A->Abort();

    delete A;
    A = (AtomicAction*) 0;

    return status;
}
    
void ComplexObjectControl::setTemplateType (GroupData& toAdd, TemplateType t) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "void ComplexObjectControl::setTemplateType ( " << t << " ) const" << endl;
#endif

    GroupData*  tmp   = &toAdd;
    const char* tType = (char*) 0;

    switch(t)
    {
    case Normal:
        tType = templateName;
	break;
    case WildCard:
	tType = wildCardName;
	break;
    case GlobalWildCard:
	tType = globalWildCardName;
	break;
    }

    while (tmp)
    {
        tmp->setObjectName(tType);
	tmp = tmp->next;
    }
}

void ComplexObjectControl::createStructure (const Uid& groupUid, GroupData*& toModify, char** locations,
					    int numberOfLocations) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;    
    debug_stream << "void ComplexObjectControl::createStructure () const" << endl;
#endif

    for (int i = 0; i < numberOfLocations; i++)
    {
	Uid u;
	GroupData* tmpGD = new GroupData;

	tmpGD->next = toModify;
	toModify = tmpGD;
	tmpGD->setHost(locations[i]);
	tmpGD->setGroupUid(groupUid);
	tmpGD->setUid(u);
    }
}

OutCome ComplexObjectControl::addTemplate (const Uid& groupUid, TemplateType t, char** locations, int numberOfLocations)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "OutCome ComplexObjectControl::addTemplate ( " << groupUid << ", " << t << " )" << endl;
#endif

    OutCome    result = NOTDONE;
    GroupData* toAdd  = (GroupData*) 0;
    
    if (numberOfLocations > 0)
    {
	createStructure(groupUid, toAdd, locations, numberOfLocations);
	result = addTemplate(groupUid, *toAdd, t);
	GroupData::deleteAll(toAdd);
    }
    
    return result;
}

OutCome ComplexObjectControl::addTemplate (const Uid& groupUid, GroupData& toAdd, TemplateType t)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "OutCome ComplexObjectControl::addTemplate ( " << groupUid << ", "
                 << t << " )" << endl;
#endif

    OutCome result = NOTDONE;
    Buffer  buff;

    if (setupDatabase())
    {
        setTemplateType(toAdd, t);
        toAdd.pack(buff);
	beginAction();
	result = GV->addTemplate(groupUid, buff);
	result = ((endAction(result) == TRUE) ? DONE : NOTDONE);
    }

    return result;
}

/*
 * If we are adding a new member to the template group then we must guarantee that
 * the replica it refers to has been created on that node. We will attempt to do
 * this before we try to add the new template to the group at the db. If we cannot
 * create this replica for *every* group which is governed by this template then
 * we will abort the operation. [To Change Later].
 *
 * If we are deleting a member from the template group then things are much simpler.
 * We do not attempt to physically remove the state from the node mentioned, but
 * simply update the view at the db.
 */

OutCome ComplexObjectControl::manipulateTemplateGroup (const Uid& templateUid, char* location, Boolean doDelete)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "OutCome ComplexObjectControl::manipulateTemplateGroup ( " << templateUid << ", " << location << ", "
                 << ((doDelete == TRUE) ? "Delete" : "Add") << " )" << endl;
#endif

    ReplicaCreate      rC;
    ReplicaDescriptor  rd;
    TemplateObject     tO;
    const ReplicaDescriptor* trail     = (ReplicaDescriptor*) 0;
    ReplicaDescriptor* groupView = (ReplicaDescriptor*) 0;
    OutCome            result    = NOTDONE;

    beginAction();
    if (GV->getView(rd, templateUid, NSInterface::nodeName, NSInterface::processID))
    {
	/* We have the group view and now need to modify each group. */

	rd.reset();
	do
	{
	    rd.getNextGroup(trail);
	    if (trail)
	    {
		if (groupView)
		{
		    delete groupView;
		    groupView = (ReplicaDescriptor*) 0;
		}
		rd.getGroupList(trail->getGroupUid(), groupView);

		if (doDelete)
		    result = ((rC.removeFromGroup(groupView, location)) ? DONE : NOTDONE);
		else
		    result = ((rC.addToGroup(groupView, location, NIL_UID, TRUE)) ? DONE : NOTDONE);
	    }
	} while ((trail) && (result == DONE));

	if (result == DONE)
	{
	    /*
	     * Have replicated *all* members of template group. Now we need to modify
	     * the template at the database.
	     */

	    rd.getTemplate(tO.tmplate);
	    if (doDelete)
		tO.deleteTemplate(location);
	    else
		tO.addTemplate(location);
	    result = replaceTemplate(templateUid, *tO.tmplate, Normal);
	}
    }

    if (result == DONE)
	result = GV->releaseGroup(templateUid, NSInterface::nodeName, NSInterface::processID);

    if (groupView)
        delete groupView;

    return ((endAction(result) == TRUE) ? DONE : NOTDONE);
}

OutCome ComplexObjectControl::deleteFromTemplate (const Uid& templateUid, char* location)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "OutCome ComplexObjectControl::deleteFromTemplate ( " << templateUid << ", " << location << " )" << endl;
#endif

    return manipulateTemplateGroup(templateUid, location, TRUE);
}

OutCome ComplexObjectControl::replaceTemplate (const Uid& groupUid, TemplateType t, char** locations, int numberOfLocations)
{ 
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "OutCome ComplexObjectControl::replaceTemplate ( " << groupUid << ", " << t << " )" << endl;
#endif    

    OutCome    result    = NOTDONE;
    GroupData* toReplace = (GroupData*) 0;
    
    if (numberOfLocations > 0)
    {
	createStructure(groupUid, toReplace, locations, numberOfLocations);
	result = replaceTemplate(groupUid, *toReplace, t);
	GroupData::deleteAll(toReplace);
    }
    
    return result;
}
    
OutCome ComplexObjectControl::replaceTemplate (const Uid& groupUid, GroupData& toReplace, TemplateType t)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "OutCome ComplexObjectControl::replaceTemplate ( " << groupUid << ", " << t << " )" << endl;
#endif
					
    OutCome result = NOTDONE;
    Buffer  buff;

    if (setupDatabase())
    {
        setTemplateType(toReplace, t);
        toReplace.pack(buff);
	beginAction();
	result = GV->replaceTemplate(groupUid, buff);
	result = ((endAction(result) == TRUE) ? DONE : NOTDONE);
    }

    return result;
}

OutCome ComplexObjectControl::deleteTemplate (const Uid& toDelete)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "OutCome ComplexObjectControl::deleteTemplate ( " << toDelete << " )" << endl;
#endif

    OutCome result = NOTDONE;

    if (setupDatabase())
    {
        beginAction();
	result = GV->deleteTemplate(toDelete);
	result = ((endAction(result) == TRUE) ? DONE : NOTDONE);
    }

    return result;
}

OutCome ComplexObjectControl::getTemplate (const Uid& groupUid, GroupData*& toGet, LockMode lMode)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "OutCome ComplexObjectControl::getTemplate ( " << groupUid << " )" << endl;
#endif

    OutCome result = NOTDONE;
    Buffer  buff;

    if (setupDatabase())
    {
        beginAction();
	
	result = ((GV->getTemplate(groupUid, buff, lMode)) ? DONE : NOTDONE);
	result = ((endAction(result)) ? DONE : NOTDONE);

	if (result == DONE)
	{
	    if (toGet)
	        delete toGet;
	    toGet = new GroupData;
	    result = ((toGet->unpack(buff)) ? DONE : NOTDONE);
	}
    }

    return result;
}

/*
 * Takes an existing group view and assembles a new one using the given
 * template. Then, the differences between the two are constructed and
 * replicas are placed on to any new nodes. We must do this when adding
 * already existing groups to a template group to make sure that the new
 * group is consistent.
 */

GroupData* ComplexObjectControl::constructDifference (const TemplateObject& tO, const ReplicaDescriptor& toMerge,
						      const Uid& oldGroup, ReplicaDescriptor*& currentView)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "GroupData* ComplexObjectControl::constructDifference ()" << endl;
#endif

    ReplicaDescriptor* temporaryGroup = (ReplicaDescriptor*) 0;
    ReplicaDescriptor* newView        = (ReplicaDescriptor*) 0;
    ReplicaDescriptor* allReplicas    = (ReplicaDescriptor*) 0;
    ReplicaDescriptor* tmpTrail       = (ReplicaDescriptor*) 0;
    GroupData* difference             = (GroupData*) 0;

    // assemble current group view.

    toMerge.getGroupList(oldGroup, currentView);
    toMerge.getAllMembers(oldGroup, allReplicas);
    if ((currentView == (ReplicaDescriptor*) 0) || (allReplicas == (ReplicaDescriptor*) 0))
        return (GroupData*) 0;

    /*
     * Assemble "new" group view. Must make replicas
     * conform to template structure.
     */

    allReplicas->setAllHostNames(nullRDName);
    temporaryGroup = tO.getTemplate();
    temporaryGroup->addToList(*allReplicas);
    temporaryGroup->getGroupList(oldGroup, newView);
    delete temporaryGroup;
    delete allReplicas;

    tmpTrail = newView;

    /* We are only interested in what is *not* present in the merging group. */

    while (tmpTrail)
    {
	Boolean            found      = FALSE;
	ReplicaDescriptor* mergeTrail = currentView;
	
	while ((!found) && (mergeTrail))
	{
	    if ((::strcmp(mergeTrail->getHost(), tmpTrail->getHost()) == 0) &&
		(mergeTrail->getUid() == tmpTrail->getUid()))
		found = TRUE;
	    else
		mergeTrail = mergeTrail->next;
	}

	/* If replica is not present then add it to the list. */

	if (!found)
	{
	    GroupData* tmpGD = new GroupData(*tmpTrail);
	    tmpGD->next = difference;
	    difference = tmpGD;
	}
	
	tmpTrail = tmpTrail->next;
    }

    if (newView)
        delete newView;

    return difference;
}

Boolean ComplexObjectControl::makeConsistent (const Uid& templateUid, const Uid& mergingUid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "Boolean ComplexObjectControl::makeConsistent ( " << templateUid << ", "
		 << mergingUid << " )" << endl;
#endif

    ReplicaDescriptor* view   = new ReplicaDescriptor;
    Boolean            result = FALSE;
    TemplateObject     tO;
    
    if (setupDatabase())
    {
	/* Get the template to check it against. */

	result = (Boolean) (getTemplate(templateUid, tO.tmplate) == DONE);
	
	if (result)
	{
	    beginAction();
	
	    /* Get the group view to be checked. */

	    result = GV->getView(*view, mergingUid, NSInterface::nodeName, NSInterface::processID);
	    
	    if (result)
	    {
	        /*
		 * Only do work if group is not already a template group.
		 */

		if (!view->isTemplate())
		{
		    ReplicaDescriptor* currentView = (ReplicaDescriptor*) 0;
		    GroupData*         difference  = constructDifference(tO, *view, mergingUid, currentView);

		    if (difference)
		    {
		        ReplicaCreate rC;
			if (!rC.copyGroup(*currentView, *difference))
			    result = FALSE;
		    }

		    if (currentView)
		        delete currentView;

		    if (difference)
		        GroupData::deleteAll(difference);
		}
		else
		{
		    /*
		     * Cannot merge template group with other template group.
		     * If it is already a member of a template group then this is
		     * ok only if the template group is the one we want to merge to anyway.
		     */

		    if (view->getGroupUid() != templateUid)
			result = FALSE;
		}
	    }

	    if (result)
		result = (Boolean) (GV->releaseGroup(mergingUid, NSInterface::nodeName, NSInterface::processID) == DONE);

	    if (result)
		result = endAction(DONE);
	    else
		result = endAction(NOTDONE);
	}
    }
    
    if (view)
	delete view;
    
    return result;
}

/*
 * At present is is not possible to group one template group with another. It
 * is necessary to state whether this is a new template group or not.
 * [Change this later.]
 * Could also do with methods which take Uid instead of StateManager.
 */
    
OutCome ComplexObjectControl::groupWithTemplate (const Uid& templateUid, const StateManager& toGroup, Boolean isNew)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "OutCome ComplexObjectControl::groupWithTemplate ( " << templateUid
                 << ", " << toGroup.get_uid() << ", " << ((isNew == TRUE) ? "New" : "Old") << " )" << endl;
#endif

    ReplicaDescriptor rd;
    OutCome           result = NOTDONE;

    if (setupDatabase())
    {
	/*
	 * If these groups are already associated with each other,
	 * then do not go any further.
	 */

	if ((isNew) || (makeConsistent(templateUid, toGroup.get_uid())))
	{
	    beginAction();
	    result = GV->mergeTemplateGroup(templateUid, toGroup.get_uid());

	    result = ((endAction(result)) ? DONE : NOTDONE);
	
	    /* Is a new group, so add it to database directly. */
	    
	    if (result == NOTDONE)
	    {
		rd.setGroupUid(toGroup.get_uid());
		rd.setUid(toGroup.get_uid());
		rd.setObjectName(toGroup.type());
		rd.setStoreRoot(toGroup.getStoreRoot());
		result = GV->addReplicas(rd, templateUid);
	    }
	}
    }

    if (result != DONE)
	error_stream << WARNING << "ComplexObjectControl::groupWithTemplate - grouping error." << endl;
    
    return result;
}

OutCome ComplexObjectControl::ungroupFromTemplate (const Uid& templateUid, const StateManager& toUnGroup)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "OutCome ComplexObjectControl::ungroupFromTemplate ( " << templateUid << ", "
                 << toUnGroup.get_uid() << " )" << endl;
#endif

    OutCome result = NOTDONE;

    if (setupDatabase())
    {
        beginAction();
	result = GV->splitTemplateGroup(templateUid, toUnGroup.get_uid());
	result = ((endAction(result)) ? DONE : NOTDONE);
    }

    return result;
}

/*
 * This method modifies the object specific information associated with
 * a template, i.e., it does not modify the template itself, only the
 * data it will be applied to. We must guarantee that any such modification
 * keeps the overall group consistent, i.e., that replica states exist on
 * given nodes.
 * [Currently is is possible to remove the only replica with any state from
 * a template group using this method.]
 */

OutCome ComplexObjectControl::modifyTemplateReplicas (const Uid& groupUid, const ReplicaDescriptor& rd, Boolean isNew, Boolean doDelete)
{
    OutCome result = NOTDONE;

    if (setupDatabase())
    {
        ReplicaDescriptor dummyRD;

        /*
	 * To guarantee consistency we (currently) must disassociate the group, modify
	 * it, and then associate it. This will ensure that any "missing" replica states
	 * are created.
	 */

	beginAction();

	result = ((GV->getView(dummyRD, groupUid, NSInterface::nodeName, NSInterface::processID) == TRUE) ? DONE : NOTDONE);

	// check it is a member of a template group.

	if ((result == DONE) && (dummyRD.isTemplate()))
	{
	    // split groups.

	    if (GV->splitTemplateGroup(dummyRD.getGroupUid(), groupUid) == DONE)
	    {
	        // now add/delete replicas.

		if (doDelete)
		    result = deleteReplicas(groupUid, rd);
		else
		    result = addReplicas(groupUid, rd);

		if (result == DONE)
		{
		    // now re-associate and create any "missing" replica states.

		    if ((isNew) || (makeConsistent(dummyRD.getGroupUid(), groupUid)))
		        result = GV->mergeTemplateGroup(dummyRD.getGroupUid(), groupUid);
		}
	    }
	}
	else
	    result = NOTDONE;

	result = ((endAction(result)) ? DONE : NOTDONE);	
    }

    return result;
}

/*
 * Currently these operations only work on non-template groups, as
 * modification of template groups in this way can cause consistency
 * problems. The template specific methods should be used otherwise.
 */
    
OutCome ComplexObjectControl::addReplicas (const Uid& groupUid, const ReplicaDescriptor& toAdd)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "OutCome ComplexObjectControl::addReplicas ( " << groupUid << " )" << endl;
#endif

    OutCome result = NOTDONE;

    if (setupDatabase())
    {
        ReplicaDescriptor dummyRD;

        beginAction();

	result = ((GV->getView(dummyRD, groupUid, NSInterface::nodeName, NSInterface::processID) == TRUE) ? DONE : NOTDONE);

	if (result == DONE)
	{
	    // make sure group is not template.

	    if (!dummyRD.isTemplate())
		result = GV->addReplicas(toAdd, groupUid);
	    else
		result = NOTDONE;
	}

	result = ((endAction(result)) ? DONE : NOTDONE);
    }

    return result;
}
    
OutCome ComplexObjectControl::deleteReplicas (const Uid& groupUid, const ReplicaDescriptor& toDelete)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "OutCome ComplexObjectControl::deleteReplicas ( " << groupUid << " )" << endl;
#endif

    OutCome result = NOTDONE;

    if (setupDatabase())
    {
        ReplicaDescriptor dummyRD;

        beginAction();

	result = ((GV->getView(dummyRD, groupUid, NSInterface::nodeName, NSInterface::processID) == TRUE) ? DONE : NOTDONE);

	// make sure group is not template.

	if (!dummyRD.isTemplate())
	    result = GV->deleteReplicas(toDelete, groupUid);
	else
	    result = NOTDONE;

	result = ((endAction(result)) ? DONE : NOTDONE);
    }

    return result;
}

OutCome ComplexObjectControl::associateGroups (const Uid& assignTo, GroupData& toAssign)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "OutCome ComplexObjectControl::associateGroups ( " << assignTo << ", GroupData& toAssign )" << endl;
#endif

    OutCome           result = NOTDONE;
    ReplicaDescriptor toUse;

    toUse = toAssign;
    if (setupDatabase())
    {
        beginAction();
	result = GV->assignCompositeObject(assignTo, toUse);
	result = ((endAction(result)) ? DONE : NOTDONE);
    }
    
    return result;
}

OutCome ComplexObjectControl::associateGroups (const Uid& assignTo, const Uid& toAssign)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "OutCome ComplexObjectControl::associateGroups ( " << assignTo << ", " << toAssign << " )" << endl;
#endif

    OutCome result = NOTDONE;

    if (setupDatabase())
    {
        beginAction();
	result = GV->assignCompositeObject(assignTo, toAssign);
	result = ((endAction(result)) ? DONE : NOTDONE);
    }
    
    return result;
}

OutCome ComplexObjectControl::disassociateGroups (const Uid& assignedTo, GroupData& assigned)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "OutCome ComplexObjectControl::disassociateGroups ( " << assignedTo << ", GroupData& assigned )" << endl;
#endif

    OutCome           result = NOTDONE;
    ReplicaDescriptor toUse;

    toUse = assigned;

    if (setupDatabase())
    {
        beginAction();
	result = GV->removeCompositeObject(assignedTo, toUse);
	result = ((endAction(result)) ? DONE : NOTDONE);
    }

    return result;
}

OutCome ComplexObjectControl::disassociateGroups (const Uid& assignedTo, const Uid& assigned)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "OutCome ComplexObjectControl::disassociateGroups ( " << assignedTo << ", " << assigned << " )" << endl;
#endif

    OutCome result = NOTDONE;

    if (setupDatabase())
    {
        beginAction();
	result = GV->removeCompositeObject(assignedTo, assigned);
	result = ((endAction(result)) ? DONE : NOTDONE);
    }

    return result;
}


#ifdef NO_INLINES
#  define COCONTROL_CC_
#  include <Replication/COControl.n>
#  undef COCONTROL_CC_
#endif
