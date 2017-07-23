/*
 * clk.c -- timer routines for lightweight processes.
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

#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include "gnulwp.h"

/* delta queue for the timer */
static struct lpq delayq;

/*
 * timersub calculate to=a-b; for timevals.  assumes a>b
 */
static void timersub (struct timeval *to, struct timeval *a, struct timeval *b)
{
	if (a->tv_usec < b->tv_usec) {
		a->tv_usec += 1000000;
		a->tv_sec--;
	}
	to->tv_usec = a->tv_usec - b->tv_usec;
	to->tv_sec = a->tv_sec - b->tv_sec;
}

/*
 * delay -- block the invoker for >t microseconds.  If t==0, reschedule.
 */
void delayp (long ut)
{
	extern struct pcb *currp;

	if (ut) {
		struct pcb *p=delayq.head, *q=0;
		struct itimerval itime;
		struct timeval t, *left = &itime.it_value;
		t.tv_sec = ut/1000000;
		t.tv_usec = ut%1000000;
		if (p) {
			getitimer (ITIMER_REAL, &itime);
			if (timercmp(left, &t, >)) {
				delayq.head = currp;
				timersub (&p->dt, left, &t);
			} else {
				for (p->dt=*left; p && !timercmp(&t, &p->dt, <); q=p, p=p->next)
					timersub (&t, &t, &p->dt);
				if (q) q->next = currp;
			}
		} else
			toq (&delayq, currp);
		currp->next = p;
		currp->dt = t;
		*left = delayq.head->dt;
		timerclear(&itime.it_interval);	/* one-shot */
		if (!timerisset(left)) left->tv_usec++;
		setitimer (ITIMER_REAL, &itime, (struct itimerval *)0);
	} else
		readyp (currp);
	reschedp ();
}

/*
 * onalarm -- process alarm signals
 */
void onalarm (void)
{
	struct pcb *p;
	struct itimerval itime;

	do {
		readyp (hoq (&delayq));
		p = delayq.head;
	} while (p && !timerisset(&p->dt));
	if (p) {
		/* start a new one-shot timer */
		timerclear(&itime.it_interval);
		itime.it_value = p->dt;
		setitimer (ITIMER_REAL, &itime, (struct itimerval *)0);
	}
}
