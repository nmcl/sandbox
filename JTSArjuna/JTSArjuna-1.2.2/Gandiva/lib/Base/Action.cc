/*
 * Copyright (C) 1994, 1995, 1996, 1997
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Action.cc,v 1.6 1997/02/11 16:47:18 nmcl Exp $
 */

#include <os/stddef.h>

#include <os/iostream.h>

#ifndef GANDIVA_H_
#   include <Gandiva.h>
#endif

#ifndef BASE_ACTION_H_
#   include <Base/Action.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#   include <Common/ClassName.h>
#endif

Action::Status Action::status() const
{
    return _status;
}

Action *Action::current()
{
    return Thread::current()->currentAction();
}

Action *Action::current(Thread *thread)
{
    if (thread == NULL)
        return NULL;
    else
        return thread->currentAction();
}

Action *Action::parent() const
{
    return _parent;
}

Boolean Action::isAncestor(const Uid &uid)
{
    Boolean ancestor = FALSE;
    Action  *action  = _parent;

    while ((action != NULL) && (! ancestor))
    {
        ancestor = (action->_uid == uid);
        action = action->_parent;
    }

    return ancestor;
}

Action *Action::find(const Uid &uid)
{
    Action *action = NULL;

    ListIterator<Action> listIterator(&_actionList);
    listIterator.head();
    while ((listIterator.content() != NULL) && (action == NULL))
    {
        if (listIterator.content()->_uid == uid)
            action = listIterator.content();
        else
            listIterator.next();
    }

    return action;
}

const Uid& Action::uid() const
{
    return _uid;
}

void Action::print(ostream& ostr) const
{
    ostr << "<Action:" << _status << ':'
         << _uid << ':' << (void*) _parent << '>';
}

const ClassName &Action::name()
{
    return Gandiva::Base::Action::name();
}

const ClassName &Action::className() const
{
    return Gandiva::Base::Action::name();
}

Action *Action::castup(Resource *resource)
{
    if (resource != NULL)
        return (Action *) resource->castup(Action::name());
    else
        return NULL;
}

void *Action::castup(const ClassName &className) const
{
    if (className == Action::name())
        return (void *) this;
    else
        return NULL;
}

Action::Action()
{
    _status = CREATED;
    _parent = NULL;

    _actionList.prepend(this);
}

Action::~Action()
{
    if (_status == RUNNING)
        cerr << "Action::~Action(): status is RUNNING!" << endl;

    _actionList.remove(this);
}

Boolean Action::begin()
{
    Boolean res = FALSE;

    if (_status == CREATED)
    {
        _status = RUNNING;

        _parent = Thread::current()->currentAction();
        Thread::current()->currentAction(this);
        if (_parent != NULL)
            _parent->_childActionList.prepend(this);

        res = TRUE;
    }
    else
        cerr << "Action::begin(): status is not CREATED!" << endl;

    return res;
}

Boolean Action::end()
{ 
    Boolean res = FALSE;

    if (_status == CREATED)
        cerr << "Action::end(): status is CREATED!" << endl;
    else if (_status == TERMINATED)
        cerr << "Action::end(): status is TERMINATED!" << endl;
    else
    {
        _status = TERMINATED;
        if (_parent != NULL)
	{
            Thread::current()->currentAction(_parent);
            _parent->_childActionList.remove(this);
	}
        else
            Thread::current()->currentAction(NULL);

        res = TRUE;
    }

    return res;
}

ostream &operator<< (ostream &ostr, const Action::Status &stat)
{
    switch (stat)
    {
        case Action::CREATED:
            ostr << "CREATED";
            break;
        case Action::RUNNING:
            ostr << "RUNNING";
            break;
        case Action::TERMINATED:
            ostr << "TERMINATED";
            break;
        default:
            ostr << "invalid!";
            break;
    }

    return ostr;
}

ostream &operator<< (ostream &ostr, const Action &action)
{
    action.print(ostr);

    return ostr;
}

List<Action> Action::_actionList;
