/*
 * lwp.c -- lightweight process creation, destruction and manipulation.
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

#include <setjmp.h>
#include <malloc.h>
#include <signal.h>
#include <stdio.h>

#include "gnulwp.h"

struct lpq schedq[MAXPRI], deadq;
struct pcb *currp;
int maxpri=0;		/* maximum priority so far */

static int oldmask;
static int growsdown (void *x) { int y; return x > (void *)&y; }

/*
 * creatp -- create a process.
 */
struct pcb *creatp (int priority, void (*entry)(int, char**, void*), int size,
		    int argc, char **argv, void *envp)
{
	struct pcb *newp;
	int *stack, x;
	extern struct pcb *currp;

	if (!(newp = (struct pcb *)malloc (sizeof(struct pcb))))
		return (0);
	size += sizeof(stkalign_t);
	if (!(stack = (int *)malloc (size)))
		return (0);
	newp->entry = entry;
	newp->argc = argc;
	newp->argv = argv;
	newp->envp = envp;
	newp->context[0] = growsdown (&x)? (size+(int)stack) & -sizeof(stkalign_t): (int)stack;
	if (MAXPRI <= priority)
		priority = MAXPRI-1;
	if (maxpri < (newp->pri = priority))
		maxpri = priority;
	newp->sbtm = stack;
	newp->size = size;
	newp->dead = 0;
	readyp (newp);
	readyp (currp);
	initp (newp);		/* architecture-dependent */
	reschedp ();
	return (newp);
}

/*
 * readyp -- put process on ready queue.  if null, assume current.
 */
void readyp (struct pcb *p)
{
	extern struct pcb *currp;
	extern struct lpq schedq[];

	if (!p)
		p = currp;
	toq (&schedq[p->pri], p);
}

/*
 * getenvp -- return back pointer to user's data
 */
void *getenvp (struct pcb *p)
{
	if (!p)
		p = currp;
	return (p->envp);
}

/*
 * reschedp -- schedule another process.  we also check for dead
 * processes here and free them.
 */
void reschedp ()
{
	extern struct lpq schedq[];
	struct pcb *nextp;
	int i;

	for (i=maxpri+1; i--; )
	{
		while (nextp = hoq (&schedq[i]))
			if (nextp->dead) {
				free (nextp->sbtm);
				free (nextp);
				if (nextp==currp)
					currp = 0;
			} else
				goto change;
	    }
change:
	/* change context? ... save current context */
	if (currp != nextp && !(currp && savep (currp->context))) {
		/* restore previous context */
		currp = nextp;
		restorep (currp->context);
	}
}

/*
 * yieldp -- yield the processor to another thread.
 */
void yieldp (void)
{
	readyp (currp);
	reschedp ();
}

/*
 * suicidep -- cause the current process to be scheduled for deletion.
 */
void suicidep (void)
{
	currp->dead = 1;
	yieldp ();
}

/*
 * destroyp -- mark a process as dead, so it will never be rescheduled.
 */
void destroyp (struct pcb *p)
{
	p->dead = 1;
}

/*
 * nullp -- a null process, always ready to run.
 * it (1) sets its priority to maximum to prevent a signal doing a
 * reschedule (2) enables signals, waits for one and handles it
 * and (3) resets its priority causing a reschedule.
 */

static void nullp (int dummy1 = 0, char** dummy2 = 0, void* dummy3 = 0)
{
	for (;;) {
		int p = prisetp (MAXPRI-1);
		sigpause (oldmask);
		prisetp (p);
	}
}

/*
 * prisetp -- set the thread's priority, returning the old.
 * if the new priority is lower than the old, we reschedule.
 */
int prisetp (int new_p)
{
	int old = currp->pri;

	if (MAXPRI <= new_p)
		new_p = MAXPRI-1;
	if (maxpri < new_p)
		maxpri = new_p;
	currp->pri = new_p;
	if (new_p < old)
		yieldp ();
	return (old);
}

/*
 * initlp -- initialise the coroutine structures
 */
struct pcb *initlp (int pri)
{
	extern struct lpq schedq[];
	extern struct pcb *currp;
	struct lpq *q;
	int i, *stack;
	struct sigvec s;

	if (!(currp = (struct pcb *)malloc (sizeof (struct pcb))))
		return (0);
	if (!(stack = (int *)malloc (64)))
		return (0);
	if (MAXPRI <= pri)
		pri = MAXPRI-1;
	if (maxpri < pri)
		maxpri = pri;
	currp->next = 0;
	currp->sbtm = stack;	/* dummy stack */
	currp->pri = pri;
	currp->dead = 0;

	for (i=MAXPRI, q=schedq; i--; q++)
		q->head = q->tail = 0;
	deadq.head = deadq.tail = 0;

	s.sv_handler = onalarm;
	s.sv_flags = SV_INTERRUPT;
	s.sv_mask = 0;
	sigvec (SIGALRM, &s, (struct sigvec *)0);

	oldmask = sigsetmask (SIGNALS);
	creatp (0, nullp, 8192, 0, 0, 0);
	return (currp);
}

/*
 * wrapp -- process entry point.
 */
void wrapp (void)
{
	extern struct pcb *currp;

	sigsetmask (SIGNALS);
	(*currp->entry) (currp->argc, currp->argv, currp->envp);
	suicidep ();
}

/* access operations for modula-2 */
int btmcurrp (void)
{
	return ((int)currp->sbtm);
}

int sizecurrp (void)
{
	return (currp->size);
}
