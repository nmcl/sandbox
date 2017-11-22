/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RepDoctor.h,v 1.1 1994/08/02 11:03:25 ndbi Exp $
 */


#ifndef REPDOCTOR_H_
#define REPDOCTOR_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef CRRECLIST_H_
#  include <CRRecList.h>
#endif

#ifndef REPINTERFACE_H_
#  include <RepInterface.h>
#endif

/*
 * Enum: Progress
 * Records the progress of RepDoctor processing
 */

enum Progress
{
        RD_WORKING, RD_COMPLETE, RD_FAILED
};

/*
 * Class: Replication Doctor (RepDoctor)
 * Responsible for the co-ordination of replication recovery
 */

class RepDoctor
{
public:
    RepDoctor(ostream&, char*, CRRecordList*);
    ~RepDoctor();
    Boolean phase1();

private:
    RepInterface *createRepInterface();
    Boolean recoverGroupView();
    Boolean getRepWork();

    // RepDoctor state
    ostream&        outStrm;
    char*           host;
    CRRecordList*   workList;

    static int           refCount;
    static Boolean       setupRepInterface;
    static RepInterface* RI;
};

#endif /* REPDOCTOR_H_ */
