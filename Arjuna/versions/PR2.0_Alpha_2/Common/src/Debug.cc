/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Debug.cc,v 1.1 1993/11/03 14:14:58 nmcl Exp $
 */

/*
 * Simple Arjuna debug system. Complicated by the need to ensure
 * the debugging streams are alive before any output is done. The scheme
 * adopted by the Iostream library is employed. This is extremely perverse!
 *
 */

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

static const char RCSid[] = "$Id: Debug.cc,v 1.1 1993/11/03 14:14:58 nmcl Exp $";

int Debug_Init::init_count = 0;		/* ensure only initialise once */

#ifndef _AIX
static Iostream_init *iosys;		/* to ensure iosystem is up */
#endif

DebugController *_arjuna_debug;		/* the debug controller */

/*
 * Initialise the debug system. May be called many times but only does
 * the actual work once.
 */

Debug_Init::Debug_Init ()
{
    if (init_count++ > 0)
	return;

#ifndef _AIX
    /* init io system */

    iosys = new Iostream_init;
#endif

    /* Create debug controller */

    if (_arjuna_debug == 0)
	_arjuna_debug = new DebugController();
}

/*
 * Closedown debug system
 */

Debug_Init::~Debug_Init ()
{
    init_count--;
    if (init_count > 0 )
	return;

    debug_stream << flush;		/* flush any pending output */
    delete _arjuna_debug;		/* final closedown */

#ifndef _AIX
    delete iosys;
#endif
}

/*
 * Provide initial values vis defaults or from environment
 */

DebugController::DebugController ()
{
    char *env_value;
    long value;

    env_value = ::getenv("DEBUG_LEVEL");
    if (env_value == NULL)
        current_level = NO_DEBUGGING;
    else
    {
        istrstream iss(env_value, strlen(env_value));

#ifdef __GNUG__
	hex(iss); iss >> value;
#else
        iss >> hex >> value;
#endif
        current_level = (DebugLevel) value;
    }

    env_value = ::getenv("DEBUG_FAC");
    if (env_value == NULL)
        current_fac = FAC_ALL;
    else
    {
        istrstream iss(env_value, strlen(env_value));

#ifdef __GNUG__
	hex(iss); iss >> value;
#else
        iss >> hex >> value;
#endif
        current_fac = (FacilityCode) value;
    }

    env_value = ::getenv("DEBUG_VIS");
    if (env_value == NULL)
        current_vis = VIS_ALL;
    else
    {
        istrstream iss(env_value, strlen(env_value));

#ifdef __GNUG__
	hex(iss); iss >> value;
#else
        iss >> hex >> value;
#endif

        current_vis = (VisibilityLevel) value;
    }

    /* create actual stream */

    dbg_stream = new Dbg_Ostream(*this);

}

DebugController::~DebugController ()
{
    dbg_stream->flush();
    delete dbg_stream;
}

void DebugController::set_all ( DebugLevel dbg, FacilityCode fac,
			        VisibilityLevel vis )
{
    dbg_stream->flush();

    current_level = dbg;
    current_fac = fac;
    current_vis = vis;
}

void DebugController::set_debuglevel ( DebugLevel dbg )
{
    dbg_stream->flush();
    current_level = dbg;
}

void DebugController::set_facility ( FacilityCode fac )
{
    dbg_stream->flush();
    current_fac = fac;
}

void DebugController::set_visibility ( VisibilityLevel vis )
{
    dbg_stream->flush();
    current_vis = vis;
}

/* 
 * This is the filter for the debugging system. It checks that the
 * values in the controller tally with the values in the stream and if
 * so outputs the buffer on cout. 
 */

#ifdef NO_NESTED_TYPES
int DebugController::filter ( const char *b, int len, open_mode m )
#else
int DebugController::filter ( const char *b, int len, ios::open_mode m )
#endif

{
    if (m != ios::out)			/* only handle output */
	return (0);

    if (b == 0 || len <= 0)		/* anything really there? */
	return (1);

    if ((current_level & dbg_stream->lvl()) &&
	 (current_fac & dbg_stream->fac()) &&
	  (current_vis & dbg_stream->vis()))
    {
	cout.write(b,len);
	cout.flush();
    }

    return (1);
}

Dbg_Ostream::Dbg_Ostream ( StreamFilter& flt )
                         : ostream(),
			   dbgLevel(FULL_DEBUGGING),
			   dbgFac(FAC_ALL),
			   dbgVis(VIS_ALL)
{
    dbgfilter = new Filterbuf(flt, ios::out);
    
    init(dbgfilter);
    
    flush();
}

Dbg_Ostream::~Dbg_Ostream ()
{
    flush();    
}

void Dbg_Ostream::set_fac ( FacilityCode fac )
{
    flush();

    dbgFac = fac;
}

void Dbg_Ostream::set_lvl ( DebugLevel lvl )
{
    flush();

    dbgLevel = lvl;
}

void Dbg_Ostream::set_vis ( VisibilityLevel vis )
{
    flush();
    
    dbgVis = vis;
}

#ifdef NO_INLINES
#  define DEBUG_CC_
#  include <Common/Debug.n>
#  undef DEBUG_CC_
#endif
