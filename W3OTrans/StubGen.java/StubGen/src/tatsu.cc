/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: tatsu.cc,v 1.17 1995/03/14 09:12:03 ngdp Exp $
 */

/*
 * Tatsu stub generator main body
 *
 */

#include <strstream.h>
#include <stdlib.h>
#include <string.h>

#ifdef SG_DEBUG
#  include "Debug.h"
#endif

#ifdef YYDEBUG
extern int yydebug;
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef GLOBALINF_H_
#  include "GlobalInf.h"
#endif

#ifndef UTILITY_H_
#  include "Utility.h"
#endif

#ifndef DECL_H_
#  include "Decl.h"
#endif

#ifndef INTERFACE_H_
#  include "Interface.h"
#endif

#ifdef USE_PURIFY
extern "C" purify_all_leaks();
#endif

void usage (char *);

static const char RCSid[] = "$Id: tatsu.cc,v 1.17 1995/03/14 09:12:03 ngdp Exp $";

static const int MAXARGS = 100;
static char *arglist [MAXARGS] = {0};

int main (int argc, char **argv)
{
    register char *ifName = 0;
    register char *compilerName = 0;
    register char **argptr = arglist;
    register char *cmdName = argv[0];
    Boolean donewarning = FALSE;
    Boolean saveIf = FALSE;
    ostrstream errorStream;

#ifdef SG_DEBUG
    DebugLevel dbglevel = NO_DEBUGGING;
    FacilityCode dbgfac = FAC_ALL;
    VisibilityLevel dbgvis = VIS_ALL;
#endif

    GlobalInfo::init();			/* set things up */

#ifdef NO_NESTED_TYPES
    GlobalInfo::compatability_mode();
#endif

    /* standard args to cpp */

    *argptr++ = "-E";
    *argptr++ = "-DSTUB";
    *argptr++ = "-D_WCHAR_T";		/* I understand wchar_t */
    *argptr++ = "-D_WCHAR_T_";
    *argptr++ = "-D_GCC_WCHAR_T";
    
    *argptr++ = "-C";

    /* Process the arg list first */

    if (argc < 2)
	usage(cmdName);

    for (argc--, argv++; argc > 0; argc--, argv++)
    {
	if ((**argv != '-') && (**argv != '+'))
	{
	    if (ifName != 0)
	    {
		if (!donewarning)
		{
		    donewarning = TRUE;
		    error("Only the first interface definition file will be processed");
		}
	    } else
		ifName = *argv;		/* assume filename */
	    continue;
	}
	else if (**argv == '-')
	{
	    /* options to be passed to cpp with no further processing */

	    *argptr++ = *argv;
	}
	else
	{
	    /* stub generation specific options */
	    
	    switch (argv[0][1])
	    {
	    case 'V':
		GlobalInfo::verboseMode(TRUE);
		break;
	    case 'C': 
		argv++;
		argc--;
		if (compilerName == 0)
		    compilerName = *argv;
		else
		{
		   errorStream << "+C option ignored : stubgen compiler already set to " 
		               << *compilerName;
			error(errorStream);
		}
		break;
	    case 'b':
	        {
		    int len = strlen(*argv);

		    if ((len == 3) && (argv[0][2] == 'c'))
			GlobalInfo::compatabilityMode();
		    break;
		}
	    case 'i':
		{
		    if (strcmp(*argv, "+idl") == 0)
		    {
			GlobalInfo::setOutputStyle(IDL_STYLE);
			*argptr++ = "-DIDL";
 
		    }
		    else
		    {
			errorStream << "illegal option " << *argv;
			error(errorStream);
		    } 
		    break;
		}
	    case 'a':
		if (::strcmp(*argv, "+arj2") == 0)
		{
		    GlobalInfo::setOutputStyle(ARJUNAII_STYLE);
		}
		break;	    
	    case 'd':
	    case 'f':
	    case 'v':
	        {
#ifdef SG_DEBUG
		    char dbg = argv[0][1];
		    int len = strlen(*argv);

		    if (len > 2)
		    {
			switch (dbg)
			{
			case 'd':
			    dbglevel = (DebugLevel)atol(*argv+2);
			    break;
			case 'f':
			    dbgfac = (FacilityCode)atol(*argv+2);
			    break;
			case 'v':
			    dbgvis = (VisibilityLevel)atol(*argv+2);
			    break;
			}
		    }
		    else
		    {
			errorStream << "-" << dbg << " option requires integer arg";
			error(errorStream);
		    }
#else
		    error("not compiled with debugging option(s) enabled");
#endif
		    
		    break;
		}
	    case 'j':
		if (::strcmp(*argv, "+java") == 0)
		{
		    GlobalInfo::setOutputStyle(CPLUSPLUS_AND_JAVA_STYLE);
		}
		else
		{
		    if (::strcmp(*argv, "+javaonly") == 0)
		    {
			GlobalInfo::setOutputStyle(JAVA_STYLE);
		    }
		    else
		    {
			if (::strcmp(*argv, "+javarmi") == 0)
			{
			    GlobalInfo::setOutputStyle(JAVA_RMI_STYLE);
			}
		    }
		}
		break;
	    case 's':
		if (strcmp(*argv, "+sh") == 0)
		{
		    GlobalInfo::setOutputStyle(SHADOWS_STYLE);
		}
		else
		{
		    argv++;
		    argc--;
		    GlobalInfo::setServerName(*argv);
		}
		break;
	    case 'y':
#ifdef YYDEBUG
		yydebug = 1;
#else
		error("not compiled with yacc debugging option(s) enabled");
#endif
		break;
	    case 'w':
		errorLog.enableWarnings(TRUE);
		break;
	    case 'S':
		saveIf = TRUE;
		break;
	    default:
		errorStream << "illegal option " << argv[0];
		error(errorStream);
	    }
	}
    }

#ifndef STUBGEN_IGNORE_COMPILER
    /* Since we use execvp just trying the compiler name might work */

    if (compilerName == 0)
       compilerName = COMPILER_NAME;

#endif

    /* Arguments collected - process the interface file */

#ifdef SG_DEBUG
	_debug.setAll(dbglevel,dbgfac,dbgvis);
#endif

    if (ifName != 0)
    {
	String temp(ifName);
	String hsuffix(".h");
	String isuffix(".i");
	
	temp = temp.rindex('.');
	
	if ((temp.size() == 2) && 
	    ((temp == hsuffix) || (temp == isuffix)))
	{
	    InterfaceDef *cif = new InterfaceDef(ifName, compilerName);

	    for (argptr = arglist; *argptr != 0; argptr++)
		cif->addCppArg(*argptr);

	    cif->process();
	    if (saveIf)
		cif->save();

	    delete cif;
	}
	else
	{
	    errorStream << "Interface name must end with either .h or .i : " << ifName;
	    error(errorStream);
	}

#ifdef USE_PURIFY
	purify_all_leaks();
#endif
	return(errorLog.noOfErrors());
    }
    else
	usage(cmdName);
}

void usage ( char * name )
{
    cerr << "usage: " << name << " [-Ddef] [-Udef] [-Iincldir] ";
#ifdef SG_DEBUG
    cerr << "[+ddebuglevel] [+ffacilitycode] [+vvisibilitylevel] ";
#endif
#ifdef YYDEBUG
	cerr << "[+y] ";
#endif
    cerr << "[+V] [+idl] [+s serverpath] [+w] [+S] [+java] [+javaonly] [+javarmi] [+arj2] interface\n";
    exit(1);
}
