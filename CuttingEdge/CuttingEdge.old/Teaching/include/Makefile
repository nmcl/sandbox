#
# Copyright 1990, 1991, 1992,
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
# $Id$
#

MAKE = make

C++ = CC

CPPFLAGS =
C++FLAGS =
LDFLAGS =

ALLDIRS  = Common Port RPC

all:
	@for i in $(ALLDIRS) ;\
	do \
		echo "Directory "$$i ;\
		(cd $$i; $(MAKE) MAKE="$(MAKE)" C++="$(C++)" CPPFLAGS="$(CPPFLAGS)" C++FLAGS="$(C++FLAGS)" LDFLAGS="$(LDFLAGS)" $@) ;\
	done

tidy clean purge:
	@for i in $(ALLDIRS) ;\
	do \
		echo "Directory "$$i ;\
		(cd $$i; $(MAKE) MAKE="$(MAKE)" $@) ;\
	done
	rm -f core *~ \#*\# *.bak *.BAK *.o
