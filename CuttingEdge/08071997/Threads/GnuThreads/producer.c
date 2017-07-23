/*
 * producer.c -- an example program to demonstrate lightweight
 * processes, semaphores and time.
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

#include <string.h>
#include <stdio.h>
#include "gnulwp.h"

struct sem *empty, *full, *term;
char c;

void f (int n, char** p1, void* dummy3 = 0)
{
        char* p = *p1;

	while (n--) {
		waits (empty);
		c = *p++;
		signals (full);
		delayp (1000000);
	}
	signals (term);
	suicidep ();
}

void g (int n, char** dummy2 = 0, void* dummy3 = 0)
{
	while (n--) {
		waits (full);
		putchar (c);
		fflush (stdout);
		signals (empty);
	}
	signals (term);
	suicidep ();
}

main ()
{
	char *str = "Hello world\n";
	int n = strlen (str);

	initlp (1);
	empty = creats (1);
	full = creats (0);
	term = creats (0);
	creatp (2, g, 2048, n, 0, 0);
	creatp (2, f, 2048, n, &str, 0);
	for (n=2; n--; )
		waits (term);
}
