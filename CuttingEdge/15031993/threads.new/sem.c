/*
 * sem.c -- semaphore manipulation.
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

#include <malloc.h>
#include "gnulwp.h"

/*
 * creats -- create a semaphore.
 */
struct sem *creats (int count)
{
	struct sem *new_s;

	if (!(new_s = (struct sem *)malloc (sizeof(struct sem))))
		return (0);
	new_s->count = count;
	new_s->q.head = new_s->q.tail = 0;
	return (new_s);
}

/*
 * signals -- signal a semaphore.  We only yield here if
 * the blocked process has a higher priority than ours'.
 */
void signals (struct sem *s)
{
	extern struct pcb *currp;

	if (s->count++ < 0) {
		struct pcb *p = hoq (&s->q);
		readyp (p);
		if (currp->pri < p->pri)
			yieldp ();
	}
}

/*
 * waits -- wait on a semaphore
 */
void waits (struct sem *s)
{
	extern struct pcb *currp;

	if (--s->count < 0) {
		toq (&s->q, currp);
		reschedp ();
	}
}
