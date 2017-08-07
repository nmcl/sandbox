/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Condition.h,v 1.2 1998/07/21 12:40:57 nmcl Exp $
 */

#ifndef COMMON_THREAD_CONDITION_H_
#define COMMON_THREAD_CONDITION_H_

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_THREAD_INITTHREADS_H_
#  include <Common/Thread/InitThreads.h>
#endif

/*
 *
 * Thread system condition class.
 *
 */

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef CORE_RESOURCE_H_
#  include <Core/Resource.h>
#endif

class ConditionData;

class ConditionTest
{
public:
    virtual Boolean test () = 0;
};
    
class Condition : public Resource
{
public:
    enum ConditionStatus { CN_DONE, CN_ERROR, CN_TIMEOUT };

    ConditionStatus wait ();
    ConditionStatus wait (unsigned long timeout);
    ConditionStatus signal ();
    ConditionStatus broadcast ();
    
    const Uid& uid () const;
    
    static Condition* create (ConditionTest* condition, Boolean canDelete);
    
protected:
    Condition (ConditionTest*, Boolean);
    virtual ~Condition();

    static Boolean _initialized;
    
    static void initialize ();

private:
    Uid _conditionId;
    ConditionData* _conditionData;
    ConditionTest* _condition;
    Boolean _canDelete;
    
public:
    static  const ClassName& name ();
    virtual const ClassName& className () const;
    
    virtual void* castup (const ClassName&) const;
    static  Condition* castup (Resource*);    
};

#endif
