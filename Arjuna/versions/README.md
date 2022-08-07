This directory contains the various releases of the Arjuna C++ project that were made available via the computer science ftp site between 1989 and 1993.

Most versions were intended to run across a range of different Unix based operating systems, later extended to include Linux. We also added support for various versions of Windows and OS/2. Though these were eventually incorporated into the mainline codebase, initially we did some bespoke versions to test how things might work. These code drops are also included.

Going through the history during checking things in, it's clear that we created a number of versions over time but failed to update the internal version number. For instance, some of the PR 2.0 Alpha releases should probably have been 2.1, 2.2 etc. but their VERSION file still refers to them as 2.0. Rather than fix this I've kept the 2.0 Alpha in the sub-directory name, but appended the date of release to the directory name too, e.g., PR2.0_Alpha_93_04_14, which means 2.0 Alpha was the release base and it was actually released on 14th of April 1993.

Note, this code was created before multi-threading was available in C++ so it is NOT thread safe even if it does compile (which it may)!

Unlikely anything will build in these directories because C++ compilers have evolved so much in the intervening years and there may be illegal code as a result.
