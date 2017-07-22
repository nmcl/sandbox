#include <locale.h>

#include <iostream.h>

#include <InterViews/style.h>
#include <InterViews/session.h>
#include <InterViews/input.h>

#include "ActionStructure.h"
#include "ConcurrentActions.h"
#include "SerialActions.h"

ActionStructure::ActionStructure() : InputHandler(NULL, Session::instance()->style())
{
    ref();
    parentStructure = NULL;
}

ActionStructure::~ActionStructure()
{
}

int ActionStructure::AddBefore(ActionStructure* as)
{
    if (parentStructure != NULL)
    {
	if (parentStructure->DoAddBefore(this, as))
	    return 1;
	else
	{
	    ActionStructure* newParentStructure;
	    newParentStructure = new SerialActions;

	    parentStructure->DoReplace(this, newParentStructure);
	    setParent(newParentStructure);
	    parentStructure->DoAddBefore(NULL, this);
	    return parentStructure->DoAddBefore(this, as);
	}
    }
    else
	return 0;
}

int ActionStructure::AddAfter(ActionStructure* as)
{
    if (parentStructure != NULL)
        if (parentStructure->DoAddAfter(this, as))
	    return 1;
	else
	{
	    ActionStructure* newParentStructure;
	    newParentStructure = new SerialActions;

	    parentStructure->DoReplace(this, newParentStructure);
	    setParent(newParentStructure);
	    parentStructure->DoAddAfter(NULL, this);
	    return parentStructure->DoAddAfter(this, as);
	}
    else
	return 0;
}

int ActionStructure::AddAbove(ActionStructure* as)
{
    if (parentStructure != NULL)
	if (parentStructure->DoAddAbove(this, as))
	    return 1;
	else
	{
	    ActionStructure* newParentStructure;
	    newParentStructure = new ConcurrentActions;

	    parentStructure->DoReplace(this, newParentStructure);
	    setParent(newParentStructure);
	    parentStructure->DoAddAbove(NULL, this);
	    return parentStructure->DoAddAbove(this, as);
	}
    else
	return 0;
}

int ActionStructure::AddBelow(ActionStructure* as)
{
    if (parentStructure != NULL)
	if (parentStructure->DoAddBelow(this, as))
	    return 1;
	else
	{
	    ActionStructure* newParentStructure;
	    newParentStructure = new ConcurrentActions;
	    
	    parentStructure->DoReplace(this, newParentStructure);
	    setParent(newParentStructure);
	    parentStructure->DoAddBelow(NULL, this);
	    return parentStructure->DoAddBelow(this, as);
	}
    else
	return 0;
}

int ActionStructure::Remove()
{
    if (parentStructure != NULL)
	parentStructure->DoRemove(this);
    else
	return 0;
}

void ActionStructure::Change()
{
    cout << "ActionStructure::Change() " << (void*) this << '\n';

    if (parentStructure != NULL)
	parentStructure->DoChange(this);

    redraw();
}

void ActionStructure::DoChange(ActionStructure* as)
{
    cout << "ActionStructures::DoChange(" << (void*) as << ") " << (void*) this << '\n';

    Change();
}

void ActionStructure::setParent(ActionStructure* as)
{
    parentStructure = as;
}
