/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: GlobalInf.h,v 1.1 1993/11/03 14:45:15 nmcl Exp $
 */

#ifndef GLOBALINF_H_
#define GLOBALINF_H_

/*
 *
 * Global information structure for stub generator. Constructed by
 * yacc generated parser as the interface definition file is scanned.
 * All members of this class are defined as static.
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

#ifndef TYPE_H_
#  include "Type.h"
#endif

#ifndef DCLCONTXT_H_
#  include "DclContxt.h"
#endif

#ifndef FILEINFO_H_
#  include "FileInfo.h"
#endif

#ifndef LINKAGE_H_
#  include "Linkage.h"
#endif

#ifndef LOCATION_H_
#  include "Location.h"
#endif

class InterfaceDef;
class TypeDescriptor;
class String;

static const int MAXLINKAGENESTING = 50;
static const int MAXCONTEXTNESTING = 50;
static const int MAXIDIRLEVEL = 50;

class GlobalInfo
{
public:
    /* public operations */

    /* Global information modification/building */

    static void addDirective (const String&);
    static void addIncDir (const String&);

    static void compatabilityMode ();

    static void init ();		/* set up - call early in main */

    static void newLine ();

    static void popContext ();
    static void popLinkage ();
    static void pushContext (DeclContext *);
    static void pushLinkage (Linkage *);

    static void putIn (Declaration *);

    static void setCurrentIf (InterfaceDef *);
    static void setCurrentLine (long);
    static void setCurrentFile (const String&);
    static void setCurrentMods (StubModifier);
    static void setInterfaceName (const String&);
    static void setServerName (const String&);

    /* Global information interrogation */

    static String getClassHdrSuff ();
    static String getClientHdrSuff ();
    static String getClientCodeSuff ();

    static InterfaceDef *getCurrentIf ();
    static DeclContext *getContext ();
    static Location getCurrentloc ();
    static Declaration *getDecl (int);
    static Linkage *getLinkage ();
    static String getMarshallCodeSuff ();
    static String getServerCodeSuff ();
    static String getServermCodeSuff ();
    static String getServerName ();
    static StubModifier getStubGlobalMods ();

    static Declaration *lookFor (const String&,
				 Type = ANY_TYPE,
				 Context = ANY_CONTEXT);
    
    static void stamp (ostream&);
    
private:
    static String clientHdrSuff;
    static String classHdrSuff;
    static String clientCodeSuff;

    static int contextLevel;
    static DeclContext *contextStack[MAXCONTEXTNESTING];
    static DeclContext *currentContext;

    static FileInfo *currentFile;
    static InterfaceDef *currentIf;
    static Linkage *currentLinkage;
    static Location currentLoc;       	/* current scanner location */

    static int dirCount;

    static FileInfo *filesRead[MAXIDIRLEVEL];

    static DeclContext globalSymbols;
    
    static DeclContext includes;	/* directly included filenames */
    static int includeLevel;
    static String *includeStack[MAXIDIRLEVEL];
    static Boolean ininclude;		/* if TRUE processing include */
    static String interfaceName;
    
    static int linkageLevel;
    static Linkage *linkageStack[MAXLINKAGENESTING];
    static String marshallCodeSuff;
    static String serverCodeSuff;
    static String servermCodeSuff;
    static String serverFileName;

    static DeclContext *typeContext;	/* for 2.0 compatability */
    static Boolean typesGlobal;
    static String version;
};

#include "GlobalInf.n"

#endif
