/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SetDebug.cc,v 1.4 1994/11/03 15:05:50 ngdp Exp $
 */

#include <Common/Debug.h>

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef Boolean_H_
#  include <Common/Boolean.h>
#endif

typedef struct {
    char *name;
    int  value;
} DBG_CODE;


DBG_CODE fac_array[] = {
 {"FAC_BASIC_RPC", FAC_BASIC_RPC},
 {"FAC_ACTION_RPC", FAC_ACTION_RPC},
 {"FAC_ATOMIC_ACTION", FAC_ATOMIC_ACTION},
 {"FAC_CONCURRENCY_CONTROL", FAC_CONCURRENCY_CONTROL},
 {"FAC_BUFFER_MAN", FAC_BUFFER_MAN},
 {"FAC_ABSTRACT_REC", FAC_ABSTRACT_REC},
 {"FAC_OBJECT_STORE", FAC_OBJECT_STORE},
 {"FAC_STATE_MAN", FAC_STATE_MAN},
 {"FAC_GENERAL", FAC_GENERAL},
 {"FAC_SHMEM", FAC_SHMEM},
 {"FAC_CRASH_RECOVERY", FAC_CRASH_RECOVERY},
 {"FAC_NAMING_AND_BINDING", FAC_NAMING_AND_BINDING},
 {"FAC_REPLICATION", FAC_REPLICATION},
 {"FAC_OBJECT_CLUSTERING", FAC_OBJECT_CLUSTERING},
 {"FAC_USER1", FAC_USER1},
 {"FAC_USER2", FAC_USER2},
 {"FAC_ALL", FAC_ALL},
 {NULL, 0}
};

DBG_CODE vis_array[] = {
 {"VIS_PRIVATE", VIS_PRIVATE},
 {"VIS_PROTECTED", VIS_PROTECTED},
 {"VIS_PUBLIC", VIS_PUBLIC},
 {"VIS_ALL", VIS_ALL},
 {NULL, 0}
};

DBG_CODE level_array[] = {
 {"NO_DEBUGGING", NO_DEBUGGING},
 {"CONSTRUCTORS", CONSTRUCTORS},
 {"DESTRUCTORS", DESTRUCTORS},
 {"CONSTRUCT_AND_DESTRUCT", CONSTRUCT_AND_DESTRUCT},
 {"FUNCTIONS", FUNCTIONS},
 {"OPERATORS", OPERATORS},
 {"FUNCS_AND_OPS", FUNCS_AND_OPS},
 {"ALL_NON_TRIVIAL", ALL_NON_TRIVIAL},
 {"TRIVIAL_FUNCS", TRIVIAL_FUNCS},
 {"TRIVIAL_OPERATORS", TRIVIAL_OPERATORS},
 {"ALL_TRIVIAL", ALL_TRIVIAL},
 {"FULL_DEBUGGING", FULL_DEBUGGING},
 {NULL, 0}
};


int curr_fac, curr_vis, curr_level;


/*
 * usage
 *    Print out usage information for the utility & exit
 *    This function does NOT return
 */
void usage()
{
    cerr << "Usage:\n";
    cerr << "   SetDebug [+|-] [FAC | VIS | LEVEL]\n";
    cerr << "   SetDebug -help [FAC | VIS | LEVEL]\n";
    exit(1);
}


/*
 * print_codes
 *    Prints out all recognized flags (from the given array)
 *    preceded by a message (which should denote the code type)
 */
void print_codes(const char *header, const DBG_CODE codes[])
{
    int i = 0;

    cerr << header << "\n";
    while (codes[i].name != NULL)
    {
	cerr << "   " << codes[i].name << "\n";
	i++;
    }
}


/*
 * read_env_var
 *    Reads from the environment var 'env_var' into an
 *    integer value. The var is expected to be in
 *    pure hexadecimal format (0-9,A-F)
 */
int read_env_var(const char *env_var, int &value)
{
    char *env_value;

    env_value = getenv(env_var);
    if (env_value == NULL)
	return FALSE;
    else
    {
	istrstream iss(env_value, strlen(env_value));
#ifdef GCC_BROKEN_IOMANIP
        hex(iss); iss >> value;
#else
        iss >> hex >> value;
#endif
	return TRUE;
    }
}

ostream& setw4(ostream& o)
{
    o.fill('0');
    o.width(4);
    return o;
}


/*
 * produce_vars
 *    This produces output which when evaluated by the current
 *    shell will set the debugging environment values to the
 *    desired settings
 */
void produce_vars( const int fac, const int vis, const int level )
{
#ifdef GCC_BROKEN_IOMANIP
    hex(cout);
#else
    cout << hex;
#endif
    if (!strcmp(getenv("SHELL"), "/bin/csh"))
    { // using csh
	cout << "setenv DEBUG_FAC   " << setw4 << fac << ";\n";
	cout << "setenv DEBUG_VIS   " << setw4 << vis << ";\n";
	cout << "setenv DEBUG_LEVEL " << setw4 << level << ";\n";
    }
    else
    { // using sh
	cout << "set DEBUG_FAC   " << setw4 << fac << "; export DEBUG_FAC;\n";
	cout << "set DEBUG_VIS   " << setw4 << vis << "; export DEBUG_VIS;\n";
	cout << "set DEBUG_LEVEL " << setw4 << level << "; export DEBUG_LEVEL;\n";
    }
}


int add(int value, int level)
{
    return (value | level);
}

int remove(int value, int level)
{
    return (value & (~level));
}


/*
 * get_current_values
 *    Gets the current values of the debugging
 *    environment variables
 */
void get_current_values()
{
    int tmp;
    
    if (read_env_var("DEBUG_FAC", tmp))
	curr_fac = tmp;
    else
	curr_fac = FAC_ALL;
    
    if (read_env_var("DEBUG_VIS", tmp))
	curr_vis = tmp;
    else
	curr_vis = VIS_ALL;
    
    if (read_env_var("DEBUG_LEVEL", tmp))
	curr_level = tmp;
    else
	curr_level = NO_DEBUGGING;
}


int find(const DBG_CODE array[], const char *name, int &value)
{
    int i = 0;

    while (array[i].name != NULL)
    {
	if (!strcmp(array[i].name, name))
	{
	    value =array[i].value;
	    return TRUE;
	}
	i++;
    }
    return FALSE;
}


int main(int argc, char *argv[])
{
    int i;
    int tmp;

    if (argc <= 1)
	usage();

    get_current_values();
    i = 1;
    while (i < argc)
    {
	if (!strcmp(argv[i], "-help"))
	{
	    i++;
	    if (i == argc)
		usage();

	    if (!strcmp(argv[i], "FAC"))
		print_codes("Recognized FAC codes:", fac_array);
	    else if (!strcmp(argv[i], "VIS"))
		print_codes("Recognized VIS codes:", vis_array);
	    else if (!strcmp(argv[i], "LEVEL"))
		print_codes("Recognized LEVEL codes:", level_array);
	    else
		usage();

	    exit(0);
	}

	else if (!strcmp(argv[i], "+"))
	{
	    i++;
	    if (i == argc)
		usage();

	    if (find(vis_array, argv[i], tmp))
	    {
		curr_vis = add(curr_vis, tmp);
		i++;
	    }
	    else if (find(fac_array, argv[i], tmp))
	    {
		curr_fac = add(curr_fac, tmp);
		i++;
	    }
	    else if (find(level_array, argv[i], tmp))
	    {
		curr_level = add(curr_level, tmp);
		i++;
	    }
	    else
		usage();
	}

	else if (!strcmp(argv[i], "-"))
	{
	    i++;
	    if (i == argc)
		usage();

	    if (find(vis_array, argv[i], tmp))
	    {
		curr_vis = remove(curr_vis, tmp);
		i++;
	    }
	    else if (find(fac_array, argv[i], tmp))
	    {
		curr_fac = remove(curr_fac, tmp);
		i++;
	    }
	    else if (find(level_array, argv[i], tmp))
	    {
		curr_level = remove(curr_level, tmp);
		i++;
	    }
	    else
		usage();
	}

	else
	{
	    if (find(vis_array, argv[i], tmp))
	    {
		curr_vis = tmp;
		i++;
	    }
	    else if (find(fac_array, argv[i], tmp))
	    {
		curr_fac = tmp;
		i++;
	    }
	    else if (find(level_array, argv[i], tmp))
	    {
		curr_level = tmp;
		i++;
	    }
	    else
		usage();
	    
	}
    }

    produce_vars(curr_fac, curr_vis, curr_level);
    return 0;
}
