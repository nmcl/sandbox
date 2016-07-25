/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Interface.h,v 1.1 1993/11/03 14:45:26 nmcl Exp $
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

static const int MAXCPPARGS = 50;
static const int MAXCPPPROGS = 8;

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

    static char *cppArgs[MAXCPPARGS];	/* args for cpp */
    static char *cppProgs[MAXCPPPROGS];	/* possible names of cpp */
 
    int cppIndex;
    char *cppFileName;

    String headerFname;
    String inputFname;
    String interfaceName;		/* name of base interface file */
};

#include "Interface.n"

#endif
