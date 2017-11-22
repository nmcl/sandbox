/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RepDescIterator.h,v 1.2 1995/09/19 10:20:24 nmcl Exp $
 */

#ifndef REPDESCITERATOR_H_
#define REPDESCITERATOR_H_

class ReplicaDescriptor;


class ReplicaDescriptorIterator
{
public:
    ReplicaDescriptorIterator (const ReplicaDescriptor& rd);
    ~ReplicaDescriptorIterator ();

    const ReplicaDescriptor* operator ()();

private:
    const ReplicaDescriptor* _list;
};

#endif
