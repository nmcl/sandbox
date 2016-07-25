/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: GlobalInf.cc,v 1.1 1993/11/03 14:47:49 nmcl Exp $
 */


/*
 * Stub generator global information
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef FILEINFO_H_
#  include "FileInfo.h"
#endif

#ifndef GLOBALINF_H_
#  include "GlobalInf.h"
#endif

#ifndef UTILITY_H_
#  include "Utility.h"
#endif

#ifndef DIRECTIVET_H_
#  include "DirectiveT.h"
#endif

#ifndef INCLUDET_H_
#  include "IncludeT.h"
#endif

#ifndef CPPLNKAGE_H_
#  include "CppLnkage.h"
#endif

#ifndef DECL_H_
#  include "Decl.h"
#endif

#ifndef DCLCONTXT_H_
#  include "DclContxt.h"
#endif

static const char RCSid[] = "$Id: GlobalInf.cc,v 1.1 1993/11/03 14:47:49 nmcl Exp $";

/*
 * Declare static members of the class here
 */

int GlobalInfo::contextLevel = 1;
DeclContext *GlobalInfo::contextStack[MAXCONTEXTNESTING] = {0};
DeclContext *GlobalInfo::currentContext = 0;

FileInfo *GlobalInfo::currentFile = 0;
InterfaceDef *GlobalInfo::currentIf = 0;
Linkage *GlobalInfo::currentLinkage = 0;
Location GlobalInfo::currentLoc("<stdin>", 0);

int GlobalInfo::dirCount = 0;
FileInfo *GlobalInfo::filesRead[MAXIDIRLEVEL] = {0};
DeclContext GlobalInfo::globalSymbols;
DeclContext GlobalInfo::includes;
int GlobalInfo::includeLevel = 0;
String *GlobalInfo::includeStack[MAXIDIRLEVEL] = {0};
Boolean GlobalInfo::ininclude = FALSE;
String GlobalInfo::interfaceName("<stdin>");

int GlobalInfo::linkageLevel = 0;
Linkage *GlobalInfo::linkageStack[MAXLINKAGENESTING] = {0};
String GlobalInfo::serverFileName("");

#ifdef SHORTNAMES
String GlobalInfo::clientHdrSuff("_S.h");
String GlobalInfo::classHdrSuff("_SC.h");
String GlobalInfo::clientCodeSuff("_C.cc");
String GlobalInfo::serverCodeSuff("_S.cc");
String GlobalInfo::servermCodeSuff("_SM.cc");
String GlobalInfo::marshallCodeSuff("_M.cc");
#else
String GlobalInfo::clientHdrSuff("_stub.h");
String GlobalInfo::classHdrSuff("_stubclass.h");
String GlobalInfo::clientCodeSuff("_client.cc");
String GlobalInfo::serverCodeSuff("_server.cc");
String GlobalInfo::servermCodeSuff("_servermain.cc");
String GlobalInfo::marshallCodeSuff("_marshall.cc");
#endif

/*
 * Table to hold typenames if in 2.0 compatability mode
 */

DeclContext *GlobalInfo::typeContext = 0;
Boolean GlobalInfo::typesGlobal = FALSE;

String GlobalInfo::version("Arjuna Stub Generator Version 3.0 alpha8 - $Date: 1993/11/03 14:47:49 $");

void GlobalInfo::addDirective ( const String& dir )
{
    String dname(constructName("_cppD_", dirCount++));
    
    putIn(new Declaration(dname,new Directive(dir)));
}

/*
 * Add a directory name specified as an option to -I to the stack of 
 * known names. Used to determine which file was really included instead
 * of just using cpp generated name (which is already expanded)
 */

void GlobalInfo::addIncDir ( const String& dirName )
{
    if (includeLevel < MAXIDIRLEVEL)
    {
	if (dirName.size() > 0)
	    includeStack[includeLevel++] = new String(dirName);
    }
    else
	fatal("internal included stack overflow");
}

void GlobalInfo::init ()
{
    contextStack[0] = currentContext = &GlobalInfo::globalSymbols;
    linkageStack[0] = currentLinkage = cppLinkage;
}

void GlobalInfo::popContext ()
{
#ifdef DEBUG
    debugStream << "popContext() : leaving : " << (void *)currentContext;
    debugStream << ", resuming :" << (void*)contextStack[contextLevel-2];
    debugStream << "\n";
    _debug.dbgFlush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    if (contextLevel == 1)
	fatal("(internal) Declaration context stack underflow");
    else
    {
	contextLevel--;
	currentContext = contextStack[contextLevel-1];
    }
}

void GlobalInfo::popLinkage ()
{
#ifdef DEBUG
    debugStream << "popLinkage() : leaving : " << (void *)currentLinkage;
    debugStream << ", resuming :" << (void*)linkageStack[linkageLevel-1];
    debugStream << "\n";
    _debug.dbgFlush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    if (linkageLevel == 0)
	fatal("(internal) Language linkage stack underflow");
    else
	currentLinkage = linkageStack[--linkageLevel];
}

void GlobalInfo::pushContext ( DeclContext *dc )
{
#ifdef DEBUG
    debugStream << "pushContext() : leaving : " << (void *)currentContext;
    debugStream << ", entering :" << (void*)dc;
    debugStream << "\n";
    _debug.dbgFlush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    if (contextLevel < MAXCONTEXTNESTING)
    {
	contextStack[contextLevel++] = dc;
	currentContext = dc;
    }
    else
	fatal("(internal) Declaration contexts nested too deep");
}

void GlobalInfo::pushLinkage ( Linkage *l )
{
#ifdef DEBUG
    debugStream << "pushLinkage() : leaving : " << (void *)currentLinkage;
    debugStream << ", entering :" << (void*)l;
    debugStream << "\n";
    _debug.dbgFlush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    if (linkageLevel < MAXLINKAGENESTING)
    {
	linkageStack[++linkageLevel] = l;
	currentLinkage = l;
    }
    else
	error("(internal) Language linkage nested too deep");
}

void GlobalInfo::putIn ( Declaration *decl )
{
    if (typesGlobal && decl && contextLevel > 1 && decl->isTypedef())
	typeContext->putIn(decl);

    currentContext->putIn(decl);
}

/*
 * A file switch has occurred. Remember the new current file name and
 * determine if this file has been visited before. If not add it to the
 * list of included files providing that the include depth
 * is not > 1.
 */

void GlobalInfo::setCurrentFile ( const String& fileName )
{
    /* Partition name into directory name & filename */

    int level;
    FileInfo *currFile = 0;
    String currDir("./");
    String dirName;
    String fname(fileName);
    
    /* Check prefix of filename against list of include dirs */

    for (level = 0; level < includeLevel; level++)
    {
	String idir(*includeStack[level]);
	int idirLen = idir.size();
	
	if (fileName(0,idirLen) == idir)
	{
	    /* found matching prefix, split filename up */
	    
	    dirName = idir + "/";
	    fname = fileName(idirLen+1); /* strip leading '/' */

	    break;
	}
    }

    /* strip leading './' if it exists (Sun cpp) */

    while (fname.size() > 2)
	if (fname(0,2) == currDir)
	    fname = fname(2);
	else
	    break;

    /* filename split - see if it has been visited before */

    for (level = 0; level < MAXIDIRLEVEL; level++)
    {
	currFile = filesRead[level];
	
	if (currFile)
	{
	    /* would like && here but cfront 2.1 cannot cope */
	    if (currFile->getDname() == dirName)
		if (currFile->getFname() == fname)
		{
		    /* seen before */
		    currentFile = currFile;
		    break;
		}
	} else
	    break;
    }
	
    if (currFile == 0)
    {
	currentFile = new FileInfo(dirName, fname);
	
	if (level < MAXIDIRLEVEL)
	    filesRead[level] = currentFile;
	else
	    fatal("(Stub Internal) file table overflow");
    }

    if (interfaceName == fileName)	/* reverting to base ? */
    {
	ininclude = FALSE;
    }
    else if (currFile == 0)
    {
	IncludeType *inctype = new IncludeType(dirName,fname,currentFile->getGeneric());
	Declaration *incdecl = new Declaration(fileName, inctype);	    

	ininclude = TRUE;	    
	putIn(incdecl);
	    
    }
    
    currentLoc.setFileName(fileName);
    errorLocation.setFileName(fileName);
    if(currentFile->warningsEnabled())
	errorLog.enableWarnings(FALSE);
    else
	errorLog.supressWarnings(FALSE);
}

void GlobalInfo::setCurrentMods ( StubModifier sm )
{
    currentFile->addStubModifier(sm);

    if (sm != STUB_NOWARNINGS)
    {
	Declaration *incDecl = lookFor(currentLoc.getFileName());

	if (incDecl)
	{
	    TypeDescriptor *incType = incDecl->getType();

	    if (incType)
		incType->addStubModifier(sm);
	}
    }
}

Declaration *GlobalInfo::lookFor ( const String& name,
				   Type type,
				   Context lookin )
{
    Declaration *d = 0;
    
    if (typesGlobal)
    {
	d = typeContext->lookFor(name, type, lookin);
	if (d == 0)
	    d = currentContext->lookFor(name, type, lookin);
    }
    else
	d = currentContext->lookFor(name, type, lookin);

    return d;
}

void GlobalInfo::stamp ( ostream& s )
{
    s << "/*\n * Automatically Generated by:\n * " << version << "\n";
    s << " *\n * Do Not Edit By Hand\n *\n */\n\n";
    s << "#ifdef STUB\n#  pragma @NoRename\n#endif\n\n";
}

#ifdef NO_INLINES
#  define GLOBALINF_CC_
#  include "GlobalInf.n"
#  undef GLOBALINF_CC_
#endif
