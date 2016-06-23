/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Filtsbuf.cc,v 1.1 1997/09/25 15:25:29 nmcl Exp $
 */

/*
 *
 * Simple filtering streambuffer. On overflow and underflow the buffer
 * is handed to a filtering class which may do with it as it sees fit.
 * Code based on example int eh 2.1 / 3.0 iostream manuals
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef FILTSBUF_H_
#  include <Common/Filtsbuf.h>
#endif

#ifndef zapeof
#  define zapeof(c) ((c)&0377)
#endif

static const char RCSid[] = "$Id: Filtsbuf.cc,v 1.1 1997/09/25 15:25:29 nmcl Exp $";

#ifdef NO_NESTED_TYPES
Filterbuf::Filterbuf ( StreamFilter& cl, open_mode m )
#else
Filterbuf::Filterbuf ( StreamFilter& cl, ios::open_mode m )
#endif
                     : streambuf(), 
		       filter(cl),
		       mode(m)
{
}

Filterbuf::~Filterbuf ()
{
}

int Filterbuf::overflow ( int c )
{
    /* check that output is allowed */
    if (!(mode & ios::out))
	return EOF;
    
    /* make sure there is a holding area */
    if (allocate() == EOF)
	return EOF;
    
    /* verify that there are no characters in get area */
    if (gptr() && gptr() < egptr())
	return EOF;
    
    /* reset get area */
    setg(0, 0, 0);
    
    /* make sure there is a put area */
    if (!pptr()) setp(base(), base());
    
    /* determine how many characters have been inserted but not consumed */
    int w = pptr() - pbase();
    
    /* if c is not EOF it is a character that must also be consumed */
    if (c != EOF)
    {
	/* we always leave space */
	*pptr() = c;
	++w;
    }
    
    /* consume characters */
    int ok = filter.filter(pbase(), w, ios::out);
    
    if (ok)
    {
	/* set up put area. Ensure there is space for one extra character */
	setp(base(), ebuf() - 1);
	return zapeof(c);
    }
    else
    {
	/* indicate error. */
	setp(0, 0);
	return EOF;
    }
}

int Filterbuf::underflow ()
{
    /* check that input is allowed */
    if (!(mode & ios::in))
	return EOF;
    
    /* make sure there is a holding area. */
    if (allocate() == EOF)
	return EOF;
    
    /* if there are characters waiting for output, then send them. */
    if (pptr() && pptr() > pbase())
	overflow(EOF);
    
    /* reset put area */
    setp(0, 0);
    
    /* setup get area */

    int blength = blen();

    if (blength > 1)
	setg(base(), base() + 1, ebuf());
    else
	setg(base(), base(), ebuf());
    
    /* produce characters */
    int ok = filter.filter(base(), blength, ios::in);
    
    if (ok)
	return zapeof(*base());
    else
    {
	setg(0, 0, 0);
	return EOF;
    }
}

int Filterbuf::sync ()
{
    if (gptr() && egptr() > gptr())
	/* no way to return characters to producer */
	return EOF;

    if (pptr() && pptr() > pbase())
    {
	/* flush waiting output */
	int c = overflow(EOF);

	if (c == EOF)
	    return EOF;
    }
    
    /* nothing to do */
    return 0;
}

