/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Test2.cc,v 1.4 1993/09/21 15:33:35 nmcl Exp $
 */

#include <System/stdlib.h>
#include <System/iostream.h>
#include <System/string.h>

#include "Object1.h"
#include "Object2.h"

#ifdef DEBUG
#include <Common/Debug.h>
#endif

int ParseArg(int argc, char *argv[], ObjectType& ot, int& iter)
{
    int err = 0;

    for (int i = 1; (i < argc) && (err == 0); i++)
    {
	if ((i < argc) && (strcmp(argv[i], "-per") == 0))
	    ot = ANDPERSISTENT;
	else if ((i < argc) && (strcmp(argv[i], "-rec") == 0))
	    ot = RECOVERABLE;
        else if ((i < argc) && (strcmp(argv[i], "-iter") == 0)) {
	    i++;
	    if (i < argc) {
		iter = atoi(argv[i]);
		if (iter<1) {
		    err = 1;
		    cout << "Number of iterations must be greater than 0.\n";
		}
	    }
	    else {
		err = 1;
		cout<<"No number given.\n";
	    }
	}
        else {
	    err = 1;
	    cout<<"Invalid parameter ["<<argv[i]<<"].\n";
	}
    }
    return err;
}

int main(int argc, char* argv[])
{
    ObjectType obj_type = RECOVERABLE;
    int niter = 5;
    int err = 0;
    err = ParseArg(argc, argv, obj_type, niter);
    if (err) exit(-1);

    cout << "\nTest alternating changes to each of two different objects\n";
    cout << "requested object type= " << obj_type << endl;
    cout << "number of iterations requested= " << niter << endl;

    Uid u1, u2;
    int res = 0;
    Object1 o1(obj_type, -1, u1, res);
    if (!res) {
	cerr << "Error creating required objects" << endl;
	exit(-1);
    }
    res = 0;
    Object2 o2(obj_type, 0, 1, u2, res);
    if (!res) {
	cerr << "Error creating required objects" << endl;
	exit(-1);
    }
    
    int o1_1;
    int o2_1, o2_2;
    
    for (int iter=0; iter<niter; iter++) {
	if (o1.change(iter, o1_1)) {
	    if (o1_1 == iter-1) {
		cout << "Object1 value changed from  " << o1_1
		     << " to " << iter << endl;
		cout << "This is correct" << endl;
	    }
	    else {
		cerr << "Object1 value changed from  " << o1_1
		     << " to " << iter
		     << " in iteration " << iter << endl;
		cerr << "This is NOT correct" << endl;
	    }
	}
	else
	    cerr << "ERROR changing object1 for iteration = " << iter << endl;

	if (o2.change(iter+1, iter+2, o2_1, o2_2)) {
	    if (o2_1 == iter && o2_2 == iter+1) {
		cout << "Object2 values changed from "
		     << o2_1 << " and " << o2_2 << " to "
		     << iter+1 << " and " << iter+2 << endl;
		cout << "This is correct." << endl;
	    }
	    else {
		cerr << "Object2 values changed from "
		     << o2_1 << " and " << o2_2 << " to "
		     << iter+1 << " and " << iter+2
		     << " in iteration " << iter << endl;
		cerr << "This is NOT correct." << endl;
	    }
	}
	else
	    cerr << "ERROR changing object2 for iteration = " << iter << endl;
	
    }

    return 0;
}




