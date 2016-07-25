/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Utility.cc,v 
 */

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef UTILITY_H_
#include <Common/Utility.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

/*
 *   The OS uses key_t values to identify its shared memory regions
 *   and semaphores. So need to convert the object's type to a key_t.
 *   For simplicity we hash the type into a signed quantity.
 */

key_t typeToKey ( const TypeName tname )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "type_toKey(\"" << tname << "\")\n" << flush;
#endif

    char buffer[1024];
    ostrstream os(buffer, 1024, ios::out);
    register char *cp;
    register unsigned long g, h = 0;

    /* Convert uid to string */

    os << tname << ends;

    cp = buffer;

    while (cp && (*cp != '\0'))
    {
        h = (h << 4) + (*cp);
        if (g = h & 0xf0000000)
        {
            h = h ^ (g >> 24);
            h = h ^ g;
        }
        cp++;
    }

    return (key_t) h;
}
 
#if defined(hpux) || defined (SYSV)
unsigned int usleep ( unsigned int useconds )
{
  struct timeval delay;

  delay.tv_sec = 0;
  delay.tv_usec = useconds;

  (void) select (0, 0, 0, 0, &delay);

  return 0;
}
#endif
