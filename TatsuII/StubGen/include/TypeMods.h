/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TypeMods.h,v 1.1 1997/06/09 19:52:18 nmcl Exp $
 */

#ifndef TMODIFIER_H_
#define TMODIFIER_H_

/*
 *
 * Enumeration type kinds of modifiers applicable to types. 
 *     
 */

#include <iostream>

using namespace std;

enum TypeModifier { MOD_SHORT = 1, MOD_LONG, MOD_SIGNED, MOD_UNSIGNED };

extern ostream& operator<< (ostream&, TypeModifier);

#endif
