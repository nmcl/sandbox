/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Debug.h,v 1.4 1995/06/26 10:24:37 ngdp Exp $
 */

#ifndef DEBUG_H_
#define DEBUG_H_

/*
 *
 * Arjuna debugging system
 *
 * The following conventions should be followed:
 *
 * 1. All debugging statements should be within an
 *    #ifdef DEBUG
 *    #endif
 *    expression.
 *
 * 2. To indicate at what level, etc. the debugging info is use
 *    debug_stream << CONSTRUCTOR << VIS_PUBLIC ... etc. That is,
 *    insert the facility code, level and visibility into the debug
 *    stream.
 *
 * 3. Use debug_stream << (whatever) to insert the actual debugging
 *    output. It is probably preferable to flush the output also. Output
 *    will only be produced if the inserted code, level, etc. match the
 *    currently requested debugging level.
 *
 * 4. To set the debug tracing level, the variable
 *    _arjuna_debug must be set to the level of tracing required.
 *    Since each level is represented by a bit in the variable,
 *    multiple levels are produced by OR-ing the fields together.
 *
 */

/* Unfortunately we need iostream.h for the definition of ios::open_mode */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

/* A filtering streambuf */

#ifndef FILTSBUF_H_
#  include <Common/Filtsbuf.h>
#endif

/* 
 * Enumerated types for FacilityCodes, Visibility levels and
 * overall debugging level. Can be inserted into a debug stream
 * to set the levels for the 'following' debugging output and also
 * used to set the overall values governing output.
 */

#ifndef FACCODE_H_
#  include <Common/FacCode.h>
#endif

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
    FUNCS_AND_OPS = FUNCTIONS | OPERATORS,
    ALL_NON_TRIVIAL = CONSTRUCT_AND_DESTRUCT | FUNCTIONS | OPERATORS,
    TRIVIAL_FUNCS = 0x0100,
    TRIVIAL_OPERATORS = 0x0200,
    ALL_TRIVIAL = TRIVIAL_FUNCS | TRIVIAL_OPERATORS,
    ERROR_MESSAGES = 0x0400,
    FULL_DEBUGGING = 0xffff
};

/*
 * The real debug controller class
 */

class Dbg_Ostream;
class ThreadMutex;

class DebugController : public StreamFilter
{
public:
    /* Constructor */

    DebugController ();
    virtual ~DebugController ();

    /* non-virtual public functions */

    void set_all (DebugLevel, FacilityCode, VisibilityLevel); 
    void set_debuglevel (DebugLevel);
    void set_facility (FacilityCode);
    void set_visibility (VisibilityLevel);

    Dbg_Ostream& stream ();

    /* virtual filtering function */
#ifdef NO_NESTED_TYPES
    virtual int filter (const char *, int, open_mode);
#else
    virtual int filter (const char *, int, ios::open_mode);
#endif

private:    
    /* Instance variables */

    Dbg_Ostream *dbg_stream;

    DebugLevel current_level;
    FacilityCode current_fac;
    VisibilityLevel current_vis;
};

/*
 * Class to handle initialisation of debug system. Follows the scheme
 * adopted by the iostream library. That is, it's complicated and perverse!
 * Handle with care...
 * One instance gets created per object file compiled.
 */

static class Debug_Init
{
public:
    Debug_Init ();
    ~Debug_Init ();

private:
    static int init_count;
} debug_init;

/*
 * The following variables are defined in Debug.cc
 */

extern DebugController *_arjuna_debug;

/*
 * We define debug_stream as a cpp macro that maps to the stream held
 * inside the debug controller. Debug_Init ensures that this exists
 * very early on in execution.
 */

#define debug_stream ((_arjuna_debug != 0)?(_arjuna_debug->stream()):(_arjuna_debug = new DebugController(),_arjuna_debug->stream()))

/*
 * A special class derived from ostream for debugging purposes.
 * It maintains the level, visibility and facility of the current
 * output been sent to the stream via operator<<.
 * This used to be done by extending the basic ostream state using
 * xalloc() and iword() but libg++ does not currently implement
 * this part of the iostream library hence this route
 */

class Dbg_Ostream : public ostream
{
public:
    Dbg_Ostream (StreamFilter&);
    ~Dbg_Ostream ();

    int fac () const;
    int lvl () const;
    int vis () const;

    void lock();
    void unlock ();
    
    void set_fac (FacilityCode);
    void set_lvl (DebugLevel);
    void set_vis (VisibilityLevel);

private:

    DebugLevel dbgLevel;
    FacilityCode dbgFac;
    VisibilityLevel dbgVis;

    streambuf *dbgfilter;
    ThreadMutex *streamLock;
};

#include <Common/Debug.n>

#endif
