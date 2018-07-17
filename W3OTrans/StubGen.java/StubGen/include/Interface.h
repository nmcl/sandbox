/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Interface.h,v 1.8 1995/02/10 12:06:36 ngdp Exp $
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

/*
 *
 * Stub generator interface class definition.
 *
 */

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef STCOMMONT_H_
#  include "StCommon.h"
#endif

#ifndef OUTPUTSTYLE_H_
#  include "OutputStyle.h"
#endif

class ostream;

static const int MAXCPPARGS = 80;
static const int MAXCPPPROGS = 8;

class InterfaceDef
{
public:
    InterfaceDef (const String&, const String&);
    ~InterfaceDef ();

    void addCppArg (const char *);

    String getHeaderFname () const;
    String getInputFname () const;

    void process ();
    void save ();

private:
    void buildCppArgs (const char *);
    Boolean parseInterface ();
    Boolean preprocess ();
    void produceAllOutput (OutputStyle);
    void produceOutput (ostream&, OutputStyle);

    static char *compilerArgs[MAXCPPARGS]; /* args for CC/g++ */
    static char *cppArgs[MAXCPPARGS];	/* args for cpp */
    static char *cppProgs[MAXCPPPROGS];	/* possible names of cpp */
 
    int compilerArgIndex;
    int cppArgIndex;

    char *cppOutputFilename;
    char *postProcessName;
    
    String headerFname;
    String inputFname;
    String interfaceName;		/* name of base interface file */
    String rootFname;
};

#include "Interface.n"

#endif
