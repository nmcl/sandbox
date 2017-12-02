/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RepDescIterator.cc,v 1.2 1995/09/19 10:21:00 nmcl Exp $
 */

#ifndef REPDESC_H_
#  include <ArjServers/RepDesc_stub.h>
#endif

#ifndef REPDESCITERATOR
#  include <ArjServers/RepDescIterator.h>
#endif


ReplicaDescriptorIterator::ReplicaDescriptorIterator (const ReplicaDescriptor& rd)
						     : _list(&rd)
{
}

ReplicaDescriptorIterator::~ReplicaDescriptorIterator ()
{
}

const ReplicaDescriptor* ReplicaDescriptorIterator::operator () ()
{
    if (_list)
    {
	const ReplicaDescriptor* ptr = _list;
	_list = _list->next;
	return ptr;
    }

    return (ReplicaDescriptor*) 0;
}
