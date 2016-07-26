/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LocalNewStudentRecordNumber.h,v 1.1 1996/08/01 09:14:42 arjuna Exp $
 */

#ifndef NEWSTUDENTRECORDNUMBER_H_
#define NEWSTUDENTRECORDNUMBER_H_

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

#ifndef SOUTCOME_H_
#  include "SOutCome.h"
#endif


class NewStudentRecordNumber : public LockManager
{
public:
    NewStudentRecordNumber (SOutCome& res, int number);
    NewStudentRecordNumber (SOutCome& res, const Uid& myUid);
    ~NewStudentRecordNumber ();

    void setNumber (SOutCome& res, int number);
    void getNumber (SOutCome& res, int &number);
    void getThenIncreaseNumber (SOutCome& res, int &number);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    int _number;
};


#endif
