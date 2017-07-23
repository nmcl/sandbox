/*
 * queue.c -- queue manipulation routines.
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

/*
 * hoq -- remove the head of the queue and return it
 */
struct pcb *hoq (struct lpq *q)
{
	struct pcb *head;

	if ((head=q->head) && !(q->head=head->next))
		q->tail = 0;
	return (head);
}

/*
 * toq -- add element to the tail of the queue
 */
void toq (struct lpq *q, struct pcb *p)
{
	if (!q->tail)
		q->head = p;
	else
		q->tail->next = p;
	q->tail = p;
	p->next = 0;
}
