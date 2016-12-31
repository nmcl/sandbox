/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Action.h,v 1.7 1998/01/12 12:53:46 nmcl Exp $
 */

#ifndef BASE_ACTION_H_
#define BASE_ACTION_H_

class Action;

#ifndef COMMON_UID_H_
#   include <Common/Uid.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef TEMPLATE_LIST_H_
#   include <Template/List.h>
#endif

class ClassName;

#ifndef COMMON_THREAD_THREAD_H_
#   include <Common/Thread/Thread.h>
#endif

class Action : public virtual Resource
{
public:
    enum Status { CREATED, RUNNING, TERMINATED };

    Status status() const;

    virtual void  addDependence(Resource *dependence) = 0;

    static Action *current();
    static Action *current(Thread *thread);

    Action        *parent() const;

    Boolean       isAncestor(const Uid &uid);
    static Action *find(const Uid &uid);

    const Uid &uid() const;

    void print(ostream &ostr) const;

    static  const ClassName &name();
    virtual const ClassName &className() const;

    static  Action *castup(Resource *resource);
    virtual void   *castup(const ClassName &className) const;

protected:
    Action();
    virtual ~Action();

    Boolean begin();
    Boolean end();

private:
    List<Action>           _childActionList;
    static List<Action>    _actionList;

    Status                 _status;
    Action                 *_parent;

    const Uid              _uid;
};

extern ostream &operator<< (ostream &ostr, const Action::Status &status);
extern ostream &operator<< (ostream &ostr, const Action &action);

#endif
