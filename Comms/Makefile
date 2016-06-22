#
# Copyright 1990, 1991,
# Computing Laboratory, University of Newcastle upon Tyne, UK.
#
# Permission to use, copy, modify and distribute this program for
# evaluation, teaching and/or research purposes only and without fee is
# hereby granted, providing that this copyright and permission notice
# appear on all copies and supporting documentation, and that similar
# conditions are imposed on any individual or organization to whom the
# program is distributed. The University of Newcastle upon Tyne makes no
# representation about the suitability of this software for any purpose.
# It is provided "as is" without express or implied warranty.
#

#
# $Id: Makefile,v 1.1 1991/08/19 18:50:49 nsmw Exp $
#
# "Comms" makefile
#

MAKE = make

CC = cc
C++ = g++

CPPFLAGS = -DDEBUG
CFLAGS =
C++FLAGS =
LDFLAGS =

MDEPFLAGS =

ALLDIRS  = Ports RPC

all:
	for i in $(ALLDIRS) ;\
	do \
		(cd $$i; $(MAKE) MAKE="$(MAKE)" CC="$(CC)" C++="$(C++)" CPPFLAGS="$(CPPFLAGS)" CFLAGS="$(CFLAGS)" C++FLAGS="$(C++FLAGS)" LDFLAGS="$(LDFLAGS)" $@) ;\
	done

tidy clean purge:
	for i in $(ALLDIRS) ;\
	do \
		(cd $$i; $(MAKE) MAKE="$(MAKE)" $@) ;\
	done
	rm -f core *~ \#*\# *.bak *.BAK *.o

install:
	for i in $(ALLDIRS) ;\
	do\
		(cd $$i; $(MAKE) MAKE="$(MAKE)" INSTALL_ROOT="$(INSTALL_ROOT)" $@) ;\
	done

depend:
	for i in $(ALLDIRS) ;\
	do \
		(cd $$i; $(MAKE) MAKE="$(MAKE)" CPPFLAGS="$(CPPFLAGS)" MDEPFLAGS="$(MDEPFLAGS)" $@) ;\
	done
