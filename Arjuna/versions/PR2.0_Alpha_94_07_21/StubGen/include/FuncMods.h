/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FuncMods.h,v 1.4 1993/03/22 09:27:19 ngdp Exp $
 */

#ifndef FUNCMODS_H_
#define FUNCMODS_H_

/*
 *
 * Enumeration for kinds of modifiers applicable to funcs. 
 *     
 */


class ostream;

enum FuncModifier { FUNC_INLINE = 1, FUNC_VIRTUAL };

extern ostream& operator<< (ostream&, FuncModifier);

#endif
