/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FuncMods.h,v 1.1 1997/06/09 19:52:13 nmcl Exp $
 */

#ifndef FUNCMODS_H_
#define FUNCMODS_H_

#include <iostream>

using namespace std;

/*
 *
 * Enumeration for kinds of modifiers applicable to funcs. 
 *     
 */

enum FuncModifier { FUNC_INLINE = 1, FUNC_VIRTUAL };

extern ostream& operator<< (ostream&, FuncModifier);

#endif
