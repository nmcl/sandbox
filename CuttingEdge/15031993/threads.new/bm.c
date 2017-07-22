#include <stdio.h>
#include <malloc.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include "gnulwp.h"
struct sem *s;

void f (int n, char** dummy2 = 0, void* dummy3 = 0)
{
	while (n--)
		yieldp ();
	signals (s);
	suicidep ();
}

void g (Thread_Par)
{
	suicidep ();
}

main ()
{
	int n, t;
	struct timeb ts, te;

	initlp (1);
	s = creats (0);
	creatp (1, f, 4096, 5000, 0, 0);
	creatp (1, f, 4096, 5000, 0, 0);
	ftime (&ts);
	waits (s);
	waits (s);
	ftime (&te);
	t = (int) (1000*(te.time-ts.time) + te.millitm - ts.millitm);
	free (s);
	printf ("Context switching: %d ms /10000\n", t);

	ftime (&ts);
	for (n=10000; n--; )
		creatp (2, g, 4096, 0, 0, 0);
	ftime (&te);
	t = (int) (1000*(te.time-ts.time) + te.millitm - ts.millitm);
	printf ("Process creation: %d ms /10000\n", t);
}
