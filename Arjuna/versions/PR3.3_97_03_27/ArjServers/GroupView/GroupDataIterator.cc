/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupDataIterator.cc,v 1.2 1995/09/19 10:20:59 nmcl Exp $
 */

#ifndef GROUPDATA_H_
#  include <ArjServers/GroupData.h>
#endif

#ifndef GROUPDATAITERATOR_H_
#  include <ArjServers/GroupDataIterator.h>
#endif


GroupDataIterator::GroupDataIterator (const GroupData& gd)
				     : _list(&gd)
{
}

GroupDataIterator::~GroupDataIterator ()
{
}

const GroupData* GroupDataIterator::operator () ()
{
    if (_list)
    {
	const GroupData* ptr = _list;
	_list = _list->next;
	return ptr;
    }

    return (GroupData*) 0;
}
