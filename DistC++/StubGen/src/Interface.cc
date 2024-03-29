/*
 * Copyright (C) 1993-1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Interface.cc,v 1.2 1998/11/20 08:28:54 nmcl Exp $
 */

/*
 * Stub generator interface class.
 *
 */

#include <string.h>

#ifdef WIN32
#  include <process.h>
#  include <strstrea.h>
#else
#  include <sys/wait.h>
#  include <unistd.h>
#  include <strstream.h>
#endif

#include <Config/Configure.h>

#include <stddef.h>
#include <fstream.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" int yyparse();

#ifdef SG_DEBUG
#   include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef GLOBALINF_H_
#  include "GlobalInf.h"
#endif

#ifndef LINKAGE_H_
#  include "Linkage.h"
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

static const char RCSid[] = "$Id: Interface.cc,v 1.2 1998/11/20 08:28:54 nmcl Exp $";

/*
 * cppArgs is nominally the same as compiler args except that it
 * includes lots of extra -D... arguments. We use it if we cannot use
 * the compiler OR using the compiler fails in some way.
 * The extra args are gleaned from experience!
 */

char *InterfaceDef::compilerArgs[MAXCPPARGS] = {0};
char *InterfaceDef::cppArgs[MAXCPPARGS] = {0};
char *InterfaceDef::cppProgs[MAXCPPPROGS] = {0};

/*
 * Public operations
 */

/*
 * Constructor: remember interface filename and create and initialise the
 * required output streams. Name will end in either .h (need cpp)
 * or .i (cpp already run). Having set cppOutputFilename change interface name
 * suffix to .h regardless
 * Also attempts to set up a set of program names to run to preprocess
 * the user's file.
 */

InterfaceDef::InterfaceDef ( const String& ifName, 
			     const String& defaultCompiler )
                           : compilerArgIndex(1),
			     cppArgIndex(1),
			     cppOutputFilename(0),
			     postProcessName(0),
			     headerFname(NullString),
			     inputFname(NullString),
			     interfaceName(ifName),
			     rootFname(NullString)

{
    int i = 0;
    int progIndex = 0;
    String if_root = ifName(0, ifName.size()-2);
    String if_tail = ifName.rindex('.');
    char *ccRootDir = ::getenv("CCROOTDIR");
#ifdef WIN32
    char *temp = ::getenv("SYS_INCLUDE");

    if (!temp)
    {
	cerr << "Error - SYS_INCLUDE not found" << endl;
	exit(0);
    }
#else
    char* temp = SYS_INCLUDE;
#endif

    GlobalInfo::setCurrentIf(this);

    GlobalInfo::addIncDir(String(temp+2));
    GlobalInfo::addIncDir("/usr/include");
    
    /* There is a lot of intuition in these calls... */
    
#ifdef STUBGEN_MC_FLAGS
    buildCppArgs(STUBGEN_MC_FLAGS);
#endif

#ifdef STUBGEN_VEND_FLAGS
    buildCppArgs(STUBGEN_VEND_FLAGS);
#endif

#ifdef STUBGEN_CC_FLAGS
    buildCppArgs(STUBGEN_CC_FLAGS);
#endif

#ifdef STUBGEN_OS_FLAGS
    buildCppArgs(STUBGEN_OS_FLAGS);
#endif

#ifdef WIN32
    buildCppArgs("/FI");
#endif

    cppArgs[cppArgIndex++] = "-D__cplusplus";

    /* Set up a selection of programs to run as "cpp" */

    for (i = 0; i < MAXCPPPROGS; i++)
	 cppProgs[i] = 0;

    if (defaultCompiler != NullString)
    {
	char *comp = new char[strlen(defaultCompiler)+1];
	strcpy(comp, defaultCompiler);	
	cppProgs[progIndex++] = comp;
    }

    if ((cppProgs[progIndex] = locateProg("cpp.ansi")) != 0)
	progIndex++;

    if ((cppProgs[progIndex] = locateProg("acpp")) != 0)
	progIndex++;
    
    if ((cppProgs[progIndex] = locateProg("cpp")) != 0)
	progIndex++;

#ifdef WIN32
    if ((cppProgs[progIndex] = locateProg("cl.exe")) != 0)
	progIndex++;
#endif

    /* Then try CCROOTDIR/cpp (as done in some versions of Sun C++) */

    if (ccRootDir != 0)
    {
	int sLen = strlen(ccRootDir);	/* sLen does not include null */
	char *cppPath = new char[sLen + 5];

	::strcpy(cppPath, ccRootDir);
	if (cppPath[sLen-1] != GlobalInfo::dirSeparator)
	    cppPath[sLen-1] = GlobalInfo::dirSeparator;

	::strcat(cppPath, "cpp");
	if ((cppProgs[progIndex] = locateProg(cppPath)) != 0)
	    progIndex++;

	::delete [] cppPath;
    }

    /* Finally any old cpp we can find */

#ifdef hpux
    if ((cppProgs[progIndex] = locateProg("/lib/cpp.ansi")) != 0)
	progIndex++;
#endif

#ifdef SYSV
    if ((cppProgs[progIndex] = locateProg("/usr/ccs/lib/cpp")) != 0)
	progIndex++;
#endif

    if ((cppProgs[progIndex] = locateProg("/usr/lib/cpp")) != 0)
	progIndex++;

    if ((cppProgs[progIndex] = locateProg("/lib/cpp")) != 0)
	progIndex++;

#ifndef WIN32
    /*
     * Some C++ compilers don't like being fed a .h file
     * So we link to a .C file based on the input name
     */

    inputFname = if_root + "_si.C";

    /* Some compilers have sloppy header files hence the casts below */

    if (::link((char *)ifName, (char *)inputFname) != 0)
    {
	ostrstream errorStream;

	errorStream << "Unable to create link from " << ifName
		    << " to " << inputFname << " errno = " << errno;
	warning(errorStream);

	// Revert to using .h file by itself (may fail) 

	inputFname = ifName;
    }
#else    
    inputFname = ifName;
#endif

    if (if_tail == (const char*) ".h")
    {		
	/* generate temorary cpp output file name */
    
	cppOutputFilename = tempnam(0, "SG_in");
    }

    interfaceName = ifName;
    rootFname = if_root;
}

/*
 * Cleanup. Ensure the temporary file generated by cpp is removed as
 * is the link to the .h
 */

InterfaceDef::~InterfaceDef ()
{
    if ((inputFname != NullString) && (inputFname != interfaceName))
	::unlink(inputFname);
    
    if (cppOutputFilename != 0)
    {
	::unlink(cppOutputFilename);

	::delete [] cppOutputFilename;
    }
    
    if (postProcessName != 0)
    {
	::unlink(postProcessName);

	::delete [] postProcessName;
    }
}

void InterfaceDef::addCppArg ( const char *arg )
{
    if ((cppArgIndex < MAXCPPARGS) && (compilerArgIndex < MAXCPPARGS))
    {
	if (arg)
	{
	    int len = ::strlen(arg);
	    char *temp = new char[len+1];

	    ::strcpy(temp,arg);
	    cppArgs[cppArgIndex++] = temp;
	    compilerArgs[compilerArgIndex++] = temp;
			
	    if ((len > 2) && (strncmp(temp,"-I",2) == 0))
	    {
		/* Include directory - note it for later reference */
				
		GlobalInfo::addIncDir(String(temp+2));
	    }
	}
    }
    else
	fatal("internal cpp argument limit exceeded");
}

/*
 * Private operations
 */

/*
 * This is magic...
 * We attempt to build in any extra -D.. etc args cpp might need based upon
 * experience
 * STUBGEN_FLAGS is a SIMPLE string of white space separated options to pass
 * on.
 * We copy the string because its constant and g++ makes it non-writable
 */

void InterfaceDef::buildCppArgs ( const char * flags )
{
    char *p = new char[::strlen(flags)+1];
    char *q = p;

    ::strcpy(p, flags);
    
    while (q && *q)
    {
	/* find option start */
	while (*q && (*q == ' ') && (*q == '\t'))
	    q++;

	p = q;
		
	if (*q)
	{
	    cppArgs[cppArgIndex++] = p;
	    
	    /* find option end */
	    while (*q && (*q != ' ') && (*q != '\t'))
		q++;
	    
	    if (*q)
		*q++ = '\0';
	}
    }
}

Boolean InterfaceDef::parseInterface ()
{
    if (preprocess())
	return (yyparse() == 0 ? TRUE : FALSE);
    else
	return FALSE;
}

/*
 * Preprocess the interface file. This simply runs cpp to get rid of
 * #includes, #defines, etc. Cpp is run with STUB defined so that the file
 * can contain special stub generator only stuff.
 * Cpp is only run if the interface name ends with .h.
 * If it ends with .i the interface is assumed to to be already preprocessed
 * We try to run several cpps in turn until one succeeds
 */

Boolean InterfaceDef::preprocess ()
{
    char **preprocArgs = 0;
    Boolean doParse = TRUE;
    char *lexInput = inputFname;
    
    struct stat statBuf;
    long pid   = 0;
    int cppCmd = 0;

#ifndef WIN32
#ifdef HAVE_UNION_WAIT
    union wait w;
#else
    int w;
#endif
#endif

    ostrstream errorStream;

    /* redirect output to file if needed */

    if (cppOutputFilename)
    {
	if (SYS_INCLUDE == 0)
	    cppArgs[cppArgIndex++] = ::getenv("SYS_INCLUDE");
	else
	    cppArgs[cppArgIndex++] = SYS_INCLUDE;
	
	cppArgs[cppArgIndex++] = "-I/usr/include";
	cppArgs[cppArgIndex++] = inputFname;
	compilerArgs[compilerArgIndex++] = inputFname;

#ifndef WIN32
	if (cppProgs[0] == 0)
	    preprocArgs = cppArgs;
	else
	    preprocArgs = compilerArgs;
#else
	preprocArgs = compilerArgs;
#endif
	while (cppProgs[cppCmd] != 0)
	{
	    doParse = TRUE;
			
	    /* Set which cpp to use */
			
	    preprocArgs[0] = cppProgs[cppCmd];

#ifndef WIN32
	    if ((pid = ::fork()) == 0)
	    {
		if (GlobalInfo::verboseMode())
		{
		    char **p = preprocArgs;
		    
		    while (*p)
		    {
			cerr << *p << " ";
			p++;
		    }
		    
		    cerr << endl;		    
		}

		if (::freopen(cppOutputFilename, "w", stdout) == 0)
		{
		    error("could not open cpp output file");
		    return FALSE;
		}
		
		/* Discard any cpp error messages */
			
		// ::freopen("/dev/null", "w", stderr);

		if (::execvp(preprocArgs[0], preprocArgs) != 0)
		{
		    errorStream << "execvp returned with errno = " << errno
				<< errno;
		    error(errorStream);
		}

		exit(errno);		
	    }

	    while (pid != wait(&w))
	    {
		/* DO NOTHING */
	    }

	    if (WEXITSTATUS(w) != 0)
#else
		if (GlobalInfo::verboseMode())
		{
		    char **p = preprocArgs;
		    
		    while (*p)
		    {
			cerr << *p << " ";
			p++;
		    }
					
		    cerr << endl;		    
		}
			
	    if (::freopen(cppOutputFilename, "w", stdout) == 0)
	    {
		error("could not open cpp output file");
		return FALSE;
	    }

	    if (spawnvp(_P_WAIT, preprocArgs[0], (const char* const*) preprocArgs) == -1)
#endif	    	    
	    {
		errorStream << preprocArgs[0] << " returned a non-zero exit status";		
		error(errorStream);
		doParse = FALSE;
	    }
	    else
	    {		
		/* Ensure some output was generated */
		
		if ((stat(cppOutputFilename, &statBuf) == 0) &&
		    (statBuf.st_size > 0))
		{
		    lexInput = cppOutputFilename;
		    break;
		}
		else
		{
		    errorStream << preprocArgs[0] << " produced no output ";
		    error(errorStream);
		    doParse = FALSE;
		}
	    }
			
	    /* Nooutput - try different cpp */
	    preprocArgs = cppArgs;
	    cppCmd++;
	}
    }

    if (doParse)
    {
#ifdef STUBGEN_POST_PROCESS_CMD
	/* Need to post process cpp output - do it via popen */
	
	FILE *cmdInput = 0;
		
	/* generate another temorary output file name */
    
	postProcessName = tempnam(0, "SG_in");

	if ((::freopen(lexInput, "r", stdin) != 0) &&
	    (::freopen(postProcessName, "w", stdout) != 0) &&
	    ((cmdInput = popen(STUBGEN_POST_PROCESS_CMD, "w")) != 0))
	{
	    char iobuff[2048];
	    int len;
	    
	    if (GlobalInfo::verboseMode())
	    {
		cerr << STUBGEN_POST_PROCESS_CMD << endl;
	    }
			
	    while ((len = fread(iobuff, 1, 2048, stdin)) > 0)
		fwrite(iobuff, len, 1, cmdInput);

	    lexInput =  postProcessName;
	}
	else
	{
	    error("could not post process cpp output file");
	    doParse = FALSE;	    
	}
		
	if (cmdInput != 0)
	    pclose(cmdInput);
#endif
	
	/* Lex reads from stdin - force that to be the preprocesed file */
     
	if (::freopen(lexInput, "r", stdin) == 0)
	{
	    error("could not reopen input file");
	    doParse = FALSE;
	}
    }

    return doParse;
}

/*
 * This is the start of the hard work. Parse the interface file.
 * If this works output the stub header and code for this interface
 * definition. 
 */

void InterfaceDef::process ()
{
    GlobalInfo::setInterfaceName(interfaceName);
    GlobalInfo::setCurrentFile(inputFname);
    GlobalInfo::setCurrentLine(1);
    
    parseInterface();
    if (errorLog.noOfErrors() == 0)
    {
	if ((GlobalInfo::getStubGlobalMods() != STUB_NORENAME) ||
	    (GlobalInfo::getOutputStyle() == IDL_STYLE))
	    produceAllOutput(GlobalInfo::getOutputStyle());
	else
	    caution("@NoRename in effect - no output produced");
    }
    errorLog.summary(cout);
}

void InterfaceDef::produceAllOutput ( OutputStyle inStyle )
{
    ofstream headerStream;
    
    switch (inStyle)
    {
    case CPLUSPLUS_STYLE:
    case SHADOWS_STYLE:
	headerFname = rootFname + "_stub.h";
	break;
    case IDL_STYLE:
	headerFname = rootFname + ".idl";
	break;
    }
    
    headerStream.open(headerFname);
    produceOutput(headerStream, inStyle);
}

void InterfaceDef::produceOutput ( ostream& onStream, OutputStyle inStyle )
{
    String defName = interfaceName.rindex('/');
    int declnum = 0;
    Declaration *currentDecl;
    Boolean linkageOpen = FALSE;
    Linkage *currentLinkage = GlobalInfo::getLinkage();
    Linkage *lastLinkage = currentLinkage;

    if (defName == NullString)
	defName = interfaceName;
    else
	defName = defName(1,0);

    defName = defName(0,defName.size() - 2).touppercase();
    defName += "_STUB_H_";

    GlobalInfo::stamp(onStream);

    /* Standard prolog for stub header file */

    onStream << "#ifndef " << defName << "\n";
    onStream << "#define " << defName << "\n\n";

    if (inStyle != IDL_STYLE)
    {
	/* First print list of standard includes */
	onStream << "#ifndef STUBGEN_TYPES_H_\n#  include <StubGen/Types.h>\n#endif\n\n";
	onStream << "class RpcBuffer;\n";
	onStream << "class ClientRpcManager;\n\n";
    }
    
    /* Walk list of known declarations producing stubs if required */

    while ((currentDecl = GlobalInfo::getDecl(declnum++)) != 0)
    {
#ifdef SG_DEBUG
	debugStream << "Processing : " << currentDecl->getName() 
	    << "(" << (void *)currentDecl << "): " << endl;
	_debug.dbgFlush(GLOBAL, FAC_STUBGEN, VIS_PUBLIC);
#endif

	Location currentLoc = currentDecl->getLoc();

	errorLocation = currentLoc;
	currentDecl->freezeDecl();
   
	if (currentLoc.getFileName() == interfaceName)
	{
	    if ((currentLinkage = currentDecl->getLinkage()) != lastLinkage)
	    {
		if (linkageOpen)
		{
		    onStream << "\n}\n";
		}
		lastLinkage = currentLinkage;
		onStream << currentLinkage << "\n{\n";
		linkageOpen = TRUE;
	    }
	    currentDecl->produceAllStubs(onStream, inStyle);
	}
    }

    if (linkageOpen)
    {
	onStream << "\n}\n";
    }

    onStream << "\n#endif  /* " << defName << " */\n" << flush;
}

/*
 * save cpp output file. Primarily for debugging purposes
 */

void InterfaceDef::save ()
{
    String fname = interfaceName(0, interfaceName.size()-2);
    String ofName = fname + ".i";
    char iobuff[4096];
    ostrstream errorStream;

    if (cppOutputFilename)
    {
	ofstream ofile(ofName);

	if (ofile)
	{
	    ifstream ifile(cppOutputFilename);

	    while (ifile != 0)
	    {
		ifile.read(iobuff, 4096);
		ofile.write(iobuff, ifile.gcount());
	    }
	} else
	{
	    errorStream << "cannot create output file: " << fname << "\n";
	    error(errorStream);
	}
    }
    else
    {
	errorLog.enableWarnings(TRUE);
	caution("+S ignored with already pre-processed interface");
    }
}

#ifdef NO_INLINES
#  define INTERFACE_CC_
#  include "Interface.n"
#  undef INTERFACE_CC_
#endif
