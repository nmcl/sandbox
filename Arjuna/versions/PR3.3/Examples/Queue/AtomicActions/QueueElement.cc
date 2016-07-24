/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: QueueElement.cc,v 1.2 1994/07/08 15:27:25 nmcl Exp $
 */


#ifndef QUEUEELEMENT_H_
#  include "QueueElement.h"
#endif


QueueElement::QueueElement (int v)
                                 : value(v)
{
}

QueueElement::~QueueElement ()
{
}
