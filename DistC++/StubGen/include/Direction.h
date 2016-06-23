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

/*
 *
 * Enumeration for marshalling direction
 *     
 */

class ostream;

enum Direction { IN, OUT };

extern ostream& operator<< (ostream&, Direction);

#endif
