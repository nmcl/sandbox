/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Interface.h,v 1.8 1993/03/22 09:27:30 ngdp Exp $
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

class ostream;

static const int maxCppArgs = 50;
static const int maxCppProgs = 8;

class InterfaceDef
{
public:
    InterfaceDef (const String&);
    ~InterfaceDef ();

    void addCppArg (const char *);

    String getHeaderFname () const;

    void process ();
    void save ();

private:
    int parseInterface ();
    int preprocess ();
    void produceStubs ();

    static char *cppArgs[maxCppArgs];	/* args for cpp */
    static char *cppProgs[maxCppProgs];	/* possible names of cpp */
 
    int cppIndex;
    char *cppFilename;

    String headerFname;
    String inputFname;
    String interfaceName;		/* name of base interface file */
};

#include "Interface.n"

#endif
