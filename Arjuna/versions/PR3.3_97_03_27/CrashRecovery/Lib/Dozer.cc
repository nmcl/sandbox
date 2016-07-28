/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Dozer.cc,v 1.1 1994/08/02 11:02:29 ndbi Exp $
 */

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef TIME_H_
#  include <System/time.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef DOZER_H_
#  include <Dozer.h>
#endif

/*
 * Class: Dozer
 * Clever sleep with exponential backoff and possible threshold function
 * firing
 */

//-Public-Functions------------------------------------------------------------

Dozer::Dozer(ostream& strm)
            : outStrm(strm),
              baseSleepTime(1),
              currentSleepTime(1),
              threshold(-1),
              noTries(0),
              firedWarning(FALSE),
              thresholdFunction(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "Dozer::Dozer()" << endl;
#endif
}

Dozer::Dozer(ostream& strm, int initialSleepTime)
            : outStrm(strm),
              baseSleepTime(initialSleepTime),
              currentSleepTime(initialSleepTime),
              threshold(-1),
              noTries(0),
              firedWarning(FALSE),
              thresholdFunction(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "Dozer::Dozer(" << initialSleepTime << ")" << endl;
#endif
}

Dozer::~Dozer()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "Dozer::~Dozer()" << endl;
#endif
}

void Dozer::doze()
{
    // Check threshold

    if ( (noTries == threshold) && (thresholdFunction) )
    {
        if ( (*thresholdFunction)() )
        {
             // if thresholdFunction returns TRUE then reset noTries
             noTries = 0;
        }
        firedWarning = TRUE;
    }
    else
    {
        sleep(currentSleepTime);
        noTries++;
        currentSleepTime = 2 * currentSleepTime;
    }
}

void Dozer::reset()
{
    noTries = 0;
    currentSleepTime = baseSleepTime;
    firedWarning = FALSE;
}

void Dozer::reset(int newBase)
{
    currentSleepTime = baseSleepTime = newBase;
    noTries = 0;
    currentSleepTime = baseSleepTime;
    firedWarning = FALSE;
}

void Dozer::setThreshold(int newThreshold)
{
    threshold = newThreshold;
}

void Dozer::setWarningFunction(warningFunction func)
{
    thresholdFunction = func;
}

Boolean Dozer::fired()
{
    return(firedWarning);
}

void Dozer::sleep(int sleepTime)
{
    time_t theTime = ::time(0);
    outStrm << "Dozer: Sleeping for " << sleepTime << " secs. Its now " 
            << ctime(&theTime) << flush;
    ::sleep(sleepTime);
    theTime = ::time(0);
    outStrm << "Dozer: Awake at " << ctime(&theTime) << flush;
}
