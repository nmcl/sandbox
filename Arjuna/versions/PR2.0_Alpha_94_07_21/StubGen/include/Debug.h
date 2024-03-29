/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Debug.h,v 1.6 1993/06/17 15:07:14 ngdp Exp $
 */

#ifndef DEBUG_H_
#define DEBUG_H_

/*
 *
 * Stub generator debug class. All debug information printed via this class.
 *
 */

class ostream;
class ostrstream;

enum FacilityCode
{
    FAC_LEX = 0x0001,
    FAC_YACC = 0x0002,
    FAC_SCOPE = 0x004,
    FAC_BASIC_TYPES = 0x0010,
    FAC_STUBGEN = 0x0020,
    FAC_SYMTAB = 0x0040,
    FAC_ALL = 0xffff
};

enum VisibilityLevel
{
    VIS_PRIVATE = 0x0001,
    VIS_PROTECTED = 0x0002,
    VIS_PUBLIC = 0x0004,
    VIS_ALL = 0xffff
};

enum DebugLevel
{
    NO_DEBUGGING = 0,
    CONSTRUCTORS = 0x0001,
    DESTRUCTORS = 0x0002,
    CONSTRUCT_AND_DESTRUCT = CONSTRUCTORS | DESTRUCTORS,
    FUNCTIONS = 0x0010,
    OPERATORS = 0x0020,
    ALL_NON_TRIVIAL = FUNCTIONS | OPERATORS | CONSTRUCT_AND_DESTRUCT,
    TRIVIAL_FUNCS = 0x0100,
    TRIVIAL_OPERATORS = 0x0200,
    ALL_TRIVIAL = TRIVIAL_FUNCS |TRIVIAL_OPERATORS,
    GLOBAL = 0x0400,
    FULL_DEBUGGING = 0xffff
};

class DebugController
{
public:
    /* Constructors and destructor */

    DebugController (DebugLevel = NO_DEBUGGING,
		     FacilityCode = FAC_ALL,
		     VisibilityLevel = VIS_ALL);

    /* non-virtual public functions */

    void dbgFlush(DebugLevel, FacilityCode, VisibilityLevel); 

    void setAll (DebugLevel, FacilityCode, VisibilityLevel); 
    void setDebugLevel (DebugLevel);
    void setFacility (FacilityCode);
    void setVisibility (VisibilityLevel);

    static ostrstream dbgStream;	/* must be public */

private:
    /* Instance variables */

    DebugLevel currentLevel;
    FacilityCode currentFac;
    VisibilityLevel currentVis;
};

/*
 * The following variables are defined in Debug.cc
 */

extern DebugController _debug;
extern ostrstream& debugStream;


#include "Debug.n"

#endif
