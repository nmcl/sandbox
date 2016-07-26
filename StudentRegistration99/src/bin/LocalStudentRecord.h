/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LocalStudentRecord.h,v 1.1 1996/08/01 09:14:43 arjuna Exp $
 */

#ifndef LOCALSTUDENTRECORD_H_
#define LOCALSTUDENTRECORD_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef OUTCOME_H_
#  include "SOutCome.h"
#endif

//@Remote, @NoMarshall
class StudentRecord : public LockManager
{
public:
    StudentRecord (enum SOutCome& res);
    StudentRecord (SOutCome& res, const Uid& myUid);
    ~StudentRecord ();

    void storeRecord (SOutCome& res, Buffer toStore);
    void fetchRecord (SOutCome& res, Buffer& toRetrieve);
    void replaceRecord (SOutCome& res, Buffer replaceWith);
    void resetRecord (SOutCome& res);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    Buffer* studentInformation;
};

#endif
