/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Dozer.h,v 1.1 1994/08/02 11:03:23 ndbi Exp $
 */

#ifndef DOZER_H_
#define DOZER_H_

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

/*
 * Class: Dozer
 * Clever sleep with exponential backoff and possible threshold function
 * firing
 */

typedef Boolean (*warningFunction) ();

class Dozer
{
public:
    Dozer(ostream&);
    Dozer(ostream&, int);
    ~Dozer();

    void doze();

    void setThreshold(int);
    void reset();
    void reset(int);
    void setWarningFunction(warningFunction);
    Boolean fired();

private:
    void sleep(int);

    ostream& outStrm;
    int baseSleepTime;
    int currentSleepTime;
    int threshold;
    int noTries;
    Boolean firedWarning;
    warningFunction thresholdFunction;
};

#endif /* DOZER_H_ */
