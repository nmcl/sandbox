/*
 * lwp.h -- prototypes and structures for lightweight processes
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
#ifndef _LWP_H
#define _LWP_H

#define Thread_Par int dummy1 = 0, char** dummy2 = 0, void* dummy3 = 0

#define MAXPRI	8

#ifndef __signal_h
#include <signal.h>
#endif
#define SIGNALS (sigmask(SIGIO)|sigmask(SIGALRM))

#ifndef mips
#ifndef _SETJMP
#include <setjmp.h>
#endif
#else
typedef int jmp_buf[16];
#endif

#ifndef _sys_time_h
#include <sys/time.h>
#endif

/* process control block.  do *not* change the position of context */
struct pcb {
	jmp_buf	context;	/* processor context area */
	void	*sbtm;		/* bottom of stack attached to it */
	int     size;           /* size of stack */
	void	(*entry)(int, char**, void*);	/* entry point */
	unsigned dead:1;	/* whether the process should be rescheduled */
	int	pri;		/* which scheduling queue we're on */
	struct timeval	dt;
	int	argc;		/* initial arguments */
	char	**argv;
	void	*envp;		/* user structure pointer */
	struct pcb	*next;
};

/* queue */
struct lpq {
	struct pcb *head, *tail;
};

/* semaphore */
struct sem {
	int count;
	struct lpq q;
};

/* signal struct */
struct siginfo {
	struct siginfo *next;
	int des;
	void (*han) (void*, int);
	void *ctx;
};

typedef double stkalign_t;

#define savep(x)	setjmp(x)
#define restorep(x)	longjmp(x, 1)

#ifdef __cplusplus
extern "C" {
#endif

#ifdef mips
int savep (jmp_buf);
void restorep (jmp_buf);
#endif

/* semaphore operations */
struct sem *creats (int);
void signals (struct sem *);
void waits (struct sem *);

/* queue operations */
void toq (struct lpq *, struct pcb *);
struct pcb *hoq (struct lpq *);

/* process operations */
#ifdef _INIT_
class GNU_Thread;
struct pcb *creatp (int, void (*func)(int, char**, GNU_Thread*), int, int, char **, void *);
#else
struct pcb *creatp (int, void (*func)(int, char**, void*), int, int, char **, void *);
#endif
void suicidep (void);
void destroyp (struct pcb *);
void readyp (struct pcb *);
void reschedp (void);
void yieldp (void);
void *getenvp (struct pcb *);
int prisetp (int);

/* access operations */
int btmcurrp (void);
int sizecurrp (void);

/* timer operations */
void delayp (long);
void onalarm (void);

/* initialisation */
struct pcb *initlp (int);
void initp (struct pcb *);
void wrapp (void);

/* signal stuff */
int sigioset (int, void (*)(), void *);
int sigioclr (int);
#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	/* _LWP_H */
