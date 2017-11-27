/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TypeMods.h,v 1.1 1993/11/03 14:46:05 nmcl Exp $
 */

#ifndef TMODIFIER_H_
#define TMODIFIER_H_

/*
 *
 * Enumeration type kinds of modifiers applicable to types. 
 *     
 */


class ostream;

enum TypeModifier { MOD_SHORT = 1, MOD_LONG, MOD_SIGNED, MOD_UNSIGNED };

extern ostream& operator<< (ostream&, TypeModifier);

#endif
