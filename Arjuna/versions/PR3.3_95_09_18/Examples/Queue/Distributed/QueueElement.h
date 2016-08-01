/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: QueueElement.h,v 1.2 1994/07/08 15:27:46 nmcl Exp $
 */

#ifndef QUEUEELEMENT_H_
#define QUEUEELEMENT_H_

#ifdef STUB
#  pragma @NoRename
#endif

// @NoRemote, @NoMarshall
class QueueElement
{
public:
    QueueElement (int);
    ~QueueElement ();
    
    int           value;
};

#endif

    
