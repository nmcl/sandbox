/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StoreList.cc,v 
 */

#ifndef STORELIST_H_
#  include "StoreList.h"
#endif

StoreList::StoreList ()
                      : useCount(0),
			instance(0),
			next(0)
{
}

StoreList::~StoreList ()
{
    if (instance)
	delete instance;
}
