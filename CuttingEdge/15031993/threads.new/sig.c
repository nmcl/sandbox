/*
 * sig.c -- share signals among threads.
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

#include <sysent.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include "gnulwp.h"

static struct siginfo *info;
static int w, n;
static fd_set fds;
static struct sigvec osv;

/*
 * iohan -- main handler for sigio.  dispatches signal to
 * first ready descriptor.
 */
static void iohan ()
{
	struct fd_set fdtmp;
	struct timeval t;
	struct siginfo *p;
	int pri;
	extern int maxpri;

	fdtmp = fds;
	t.tv_sec = t.tv_usec = 0;
	select (w, &fdtmp, (fd_set *)0, (fd_set *)0, &t);
	for (p=info; p; p=p->next)
		if (FD_ISSET(p->des, &fdtmp)) {
			p->han (p->ctx, p->des);
			break;
		}
}

/*
 * sigioset -- install handler for SIGIO
 */
int sigioset (int fd, void (*handler) (void*, int), void *context)
{
	struct siginfo *p;
	struct sigvec sv;

	if (!n) {
		w = getdtablesize ();
		FD_ZERO(&fds);
		sv.sv_handler = iohan;
		sv.sv_mask = 0;
		sv.sv_flags = SV_INTERRUPT;
		sigvec (SIGIO, &sv, &osv);
	}
	if (n++ == w)
		return (-1);
	p = (struct siginfo *)malloc (sizeof(struct siginfo));
	p->next = info;
	p->han = handler;
	p->des = fd;
	p->ctx = context;
	info = p;
	FD_SET(fd, &fds);
	return (0);
}

/*
 * sigioclr -- remove handler for SIGIO
 */
int sigioclr (int fd)
{
	struct siginfo *p, *q;

	for (p=info, q=0; p; q=p, p=p->next)
		if (p->des == fd)
			break;
	if (!p) return (-1);

	if (q) q->next = p->next;
	else info = p->next;
	FD_CLR(fd, &fds);

	free (p);
	if (!--n)
		sigvec (SIGIO, &osv, (struct sigvec *)0);
	return (0);
}
