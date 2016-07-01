The old C++SIM used to be called Process and was developed for the Replica Placement work. Hence why replica placement code appears within the body of this source tree. Eventually the simulation libraries were factored out and became C++SIM.

NOTE: at some point maybe this can be made to build, but it'll require replacement of the dependency on Sun's lwp package, so probably not really an effort worth doing because you might as well look at the latest C++SIM!

----

D. McCue, M. Little	Computing Laboratory
29 November 1991	University of Newcastle upon Tyne, NE1 7RU, England

This directory contains a set of c++ class definitions that mimic the
process-based simulation facilities of SIMULA.  

The co-routine facility of Simula is implemented by Sun threads.
Classes are provided for various random number distributions.

The following classes are defined:

    Process	- An abstract class that exports the major functions
		  of the Simula class, process.  To use, derive your own
		  class from the class Process.  The pure virtual function,
		  Body, is the "main" procedure of the class.  Note that,
		  like Simula, a process is not scheduled to run when it is
		  created.  It must be explicitly 'activated'.

    ProcessList	- A list class for processes that (by default) orders
		  the elements by event time.

    ProcessIterator - An iterator class for ProcessList


