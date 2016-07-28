/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AADoctor.h,v 1.2 1994/08/04 08:59:49 ndbi Exp $
 */

#ifndef AADOCTOR_H_
#define AADOCTOR_H_

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef CRRECLIST_H_
#  include <CRRecList.h>
#endif

#ifndef AADOCTORSLEEP
#  define AADOCTORSLEEP 30
#endif

/*
 * Class: AADoctor
 * Responsible for the co-ordination of atomic action recovery
 */

class AADoctor
{
public:
    AADoctor(ostream&, char*, CRRecordList*);
    ~AADoctor();

    Boolean phase1();
    Boolean phase2();

private:
    int findUnresolvedAAs(Boolean);
    int findUnresolvedSAAs(Boolean);
    int findShadowedStates(Boolean);

    // AADoctor state
    ostream&      outStrm;
    char*         host;
    CRRecordList* workList;
};

#endif /* AADOCTOR_H_ */
