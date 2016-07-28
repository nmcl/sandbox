/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupDataIterator.h,v 1.2 1995/09/19 10:20:22 nmcl Exp $
 */

#ifndef GROUPDATAITERATOR_H_
#define GROUPDATAITERATOR_H_

class GroupData;


class GroupDataIterator
{
public:
    GroupDataIterator (const GroupData& gd);
    ~GroupDataIterator ();

    const GroupData* operator ()();

private:
    const GroupData* _list;
};

#endif
