/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecListI.h,v 1.4 1993/03/22 09:16:13 ngdp Exp $
 */

#ifndef RECLISTI_H_
#define RECLISTI_H_

/*
 *
 * RecordList Iterator Class
 *
 */

#ifndef RECLIST_H_
#  include "RecList.h"
#endif

class AbstractRecord;

class RecordListI
{
public:
    /* Constructors and destructor */

    RecordListI (const RecordList&);

    /* non-virtual member functions and operators */

    void restart ();
    AbstractRecord *operator() ();

private:
    /* private state variables */

    AbstractRecord *curElem;
    const RecordList& curList;
};

#include "RecListI.n"

#endif







