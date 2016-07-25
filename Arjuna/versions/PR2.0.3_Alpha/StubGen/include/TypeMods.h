/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TypeMods.h,v 1.4 1993/03/22 09:27:50 ngdp Exp $
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
