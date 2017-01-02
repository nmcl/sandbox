/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPlus_SpecificData.cc,v 1.3 1998/01/19 10:35:11 nmcl Exp $
 */

/*
 * Implementation of thread specific state for MSD threads
 */

#include <os/msd/thread.hh>
#include <os/msd/tsd.hh>
#include <os/msd/time.hh>
#include <os/msd/sighandler.hh>

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

class OrbPlus_SpecificDataImple : public ThreadDataImple
{
public:
    OrbPlus_SpecificDataImple ();
    ~OrbPlus_SpecificDataImple ();

    virtual Boolean setSpecific    (void* threadData);
    virtual Boolean getSpecific    (void*& threadData);
    virtual Boolean removeSpecific ();    
    
private:
    MSD_ThreadSpecificData<void> _data;
};


OrbPlus_SpecificDataImple::OrbPlus_SpecificDataImple ()
{
    valid(TRUE);
}

OrbPlus_SpecificDataImple::~OrbPlus_SpecificDataImple ()
{
    removeSpecific();
    valid(FALSE);
}

Boolean OrbPlus_SpecificDataImple::getSpecific ( void *& anyData )
{
    if (valid())
    {
	anyData = _data;
	return TRUE;
    }
    
    return FALSE;
}

Boolean OrbPlus_SpecificDataImple::setSpecific ( void *anyData )
{
    if (valid())
    {
	_data = anyData;

        return TRUE;
    }
    
    return FALSE;
}

Boolean OrbPlus_SpecificDataImple::removeSpecific ()
{
    if (valid())
    {
	_data = 0;
	
	return TRUE;
    }

    return FALSE;
}

ThreadDataImple* ThreadDataImple::create ()
{
    OrbPlus_SpecificDataImple* temp = new OrbPlus_SpecificDataImple();

    if (temp)
    {
	if (!temp->valid())
	{
	    delete temp;
	    temp = 0;
	}
    }
	
    return temp;
}
