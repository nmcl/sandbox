/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Direction.h,v 1.1 1997/09/25 15:29:54 nmcl Exp $
 */

#ifndef DIRECTION_H_
#define DIRECTION_H_

#include <iostream>

using namespace std;

/*
 *
 * Enumeration for marshalling direction
 *     
 */

enum Direction { IN, OUT };

extern ostream& operator<< (ostream&, Direction);

#endif
