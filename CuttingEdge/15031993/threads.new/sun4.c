/*
 * init.c -- lightweight process initialisation for sun4.
 * Copyright (C) 1991 Stephen Crane.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 1, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * author: Stephen Crane, (jsc@doc.ic.ac.uk), Department of Computing,
 * Imperial College of Science, Technology and Medicine, 180 Queen's
 * Gate, London SW7 2BZ, England.
 */

#include "gnulwp.h"

#include <setjmp.h>

/*
 * initp -- initialise a new process's context.  Stack pointer in
 * newp->context[0], because it's easier for the SPARC code.
 */

void initp (struct pcb *newp)
{
	static jmp_buf *cpp;
	extern struct pcb *currp;
	
	/* preserve cpp for new context */
	cpp = &newp->context;
	if (!savep (currp->context)) {
		/* create new context */		
		/* flush registers */
		asm ("ta	0x03");
		/* %o0 <- newp */
		asm ("ld	[%fp+0x44], %o0");
		/* %o1 <- newp->context[0] */
		asm ("ld	[%o0], %o1");
		/* create min frame on new stack */
		asm ("save	%o1,-96, %sp");
		if (!savep (*cpp))
			restorep (currp->context);
		wrapp ();
	}
}
