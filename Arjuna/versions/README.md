This directory contains the various releases of Arjuna that were made available via the computer science ftp site between 1989 and 1993.

Most versions were intended to run across a range of different Unix based operating systems, later extended to include Linux. We also extended to add support for various versions of Windows and OS/2. Though these were eventually incorporated into the mainline codebase, initially we did some bespoke versions to test how things might work. These bespoke code drops are also included here.

Going through the history during checking things in, it's clear that we created a number of versions over time but failed to update the internal version number. For instance, some of the PR 2.0 Alpha releases should probably have been 2.1, 2.2 etc. but their VERSION file still refers to them as 2.0. Rather than fix this, I've simply added a micro increment to the directory structure to distinguish them, preferring to maintain the (broken) history as much as possible.

Note, this code was created before multi-threading was available in C++ so NOT thread safe!
