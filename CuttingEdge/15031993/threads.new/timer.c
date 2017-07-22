/*
 * timer.c -- an example program to demonstrate lightweight
 * processes and time.
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

#include <sysent.h>
#include <stdio.h>
#include "gnulwp.h"

struct sem *ends;

void f (Thread_Par)
{
	int i; char ch='O';
	for (i=1; i<1000; i++) {
		delayp (25*i);
		write (1, &ch, 1);
	}
	signals (ends);
	suicidep ();
}

void g (Thread_Par)
{
	int i; char ch=' ';
	for (i=1; i<1000; i++) {
		delayp (35*i);
		write (1, &ch, 1);
	}
	signals (ends);
	suicidep ();
}

main ()
{
	initlp (1);
	ends = creats (0);
	creatp (2, g, 8192, 0, 0, 0);
	creatp (2, f, 8192, 0, 0, 0);
	waits (ends);
	waits (ends);
}
