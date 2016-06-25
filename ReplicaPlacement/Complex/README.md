/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

This is a simple distributed system simulation which is used to test the
placement policies for replicated objects. To run it simple type Make and
when it has compiled issue one of the following commands:

	Simulation Random (to run with the random placement policy)
		or
	Simulation Simple (to run with the simple placement policy)

The network consists of various nodes (Machines) which are host to replicated
objects (Object). These objects form replica groups and are used by clients
(Client processes). The clients invoke operations on the objects within atomic action.
The operations are of one of three types: read, write, and unlock. Read and write
both require read or write quora respectively, whereas unlock will operate on all available
objects used by this client.
Machine fail at intervals, and when this occurs locks are lost on them. Clients are informed
of such failures and must determine whether or not they still have a quorum to continue to
use the replica group. If they do not then this action must abort to be restarted later.
The placement policies simple determine the number of objects in a group and their initial
locations (Machines on which they are located).

The classes provided include:

	AC - This is a representation of the Available Copies replication protocol (a read
	     quorum is any one available replica, and a write quorum is all available
	     replicas).

	Client - Instances of this class represent the clients which invoke operations on
		 the replica groups.

	Machine - Instances of this class represent the machines (nodes) on which replicated
		  objects reside. These machines are subject to simulation failures, causing
		  the (temporary) loss of any objects placed on them.

	MainBody - This is the main body of code, which starts all of the processes involved
		   in the simulation.

	Object - Instances of this class represent the objects which form the groups which clients
		 use. They determine whether or not they can currently accept requests from clients
		 (i.e., whether they are currently locked in a conflicting way).

	Placement - This is the Placement Polciy process. It provides operations to make use of
		    the suitable policy (Random or Simple) and to initialize the simulated
		    distributed system accordingly, i.e., it also creates and places the objects.

	Protocol - This is the base class from which all replication protocols MUST be derived.

	RandomPolicy - This represents the Random Placement Policy.

	SimplePolicy - This represents the Simple Placement Policy.

	Simulation - This is the 'main' part of the simulation. It initializes the threads package
		     and checks that the program is being invoked correctly. It then suspends
		     main's thread so that the rest of the simulation processes can become active.
