/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: String.cc,v 1.10 1993/03/22 09:31:28 ngdp Exp $
 */

/*
 * Stub generator string class. Strings are reference counted entities
 * to save space. To avoid problems dereferencing NULL the internal
 * representation (the struct string) of a String is ALWAYS allocated
 * and contains a 1 byte array so even the null String occupies space.
 *
 */

#include <iostream.h>
#include <ctype.h>

/* toupper is sometimes a macro */

#ifndef toupper
extern "C" int toupper (int);
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

static const char RCSid[] = "$Id: String.cc,v 1.10 1993/03/22 09:31:28 ngdp Exp $";

/* The Null String */

String NullString;

/*
 * Public constructors and destructor
 */

String::String ()
{
    sp = new string;
    sp->s = new char[1];
    sp->s[0] = 0; 
    sp->ref = 1;
    sp->slen = 0;
}

/*
 * String from C-style char *
 */

String::String ( const char* cstring )
{
    register int len = (cstring != 0 ? ::strlen(cstring)+1 : 1);

    sp = new string;
    sp->s = new char[len];
    if (cstring != 0)
	::strcpy (sp->s, cstring);
    sp->s[len-1] = '\0';
    sp->ref = 1;
    sp->slen = len-1;
}

String::String ( const unsigned char* ucstring )
{
    const char *cstring = (const char *)ucstring;
    register int len = (cstring != 0 ? ::strlen(cstring)+1 : 1);

    sp = new string;
    sp->s = (char *)new unsigned char[len];
    if (cstring != 0)
	::strcpy (sp->s, cstring);
    sp->s[len-1] = '\0';
    sp->ref = 1;
    sp->slen = len-1;
}

/*
 * Copy constructor - new String from old
 */

String::String ( const String& olds )
{
    sp = olds.sp;
    if (sp)
	sp->ref++;
}

/*
 * Partial copy constructor - new String is prefix of old
 */

String::String ( const String& olds, int length )
{
    sp = new string;

    /* If length to copy == 0 or is > than size of old String copy all */

    if ((length == 0) || (olds.size() <= length))
	length = olds.size()+1;

    sp->s = new char[length];
    strncpy (sp->s, olds.sp->s, length-1);
    sp->s[length-1] = '\0';
    sp->ref = 1;
    sp->slen = length-1;
}

String::~String ()
{
    if (sp)
    {
	if (--sp->ref == 0)
	{
	    if (sp->s)
		delete [] sp->s;
	    delete sp;
	}
    }
}

/*
 * Index and rindex on strings use the standard C library functions,
 * hence the use of the scope resolution operator. They return NEWLY
 * ALLOCATED String objects and rely on a properly initialised String object.
 */

String String::index ( const char ch ) const
{
    register char *cp = ::strchr(sp->s, ch);

    if(cp)
	return String(cp);
    else
	return NullString;
}

void String::printout ( ostream& s ) const
{
    if (sp && (sp->slen > 0) && sp->s)
	s << sp->s;
}

String String::rindex ( const char ch ) const
{
    register char *cp = ::strrchr(sp->s, ch);

    if(cp)
	return String(cp);
    else
	return NullString;
}

/*
 * Return upper-case version of this
 */

String String::touppercase () const 
{
    register char *cp = sp->s;
    register char *start = new char[sp->slen+1];
    register char *temp = start;
    String rVal = NullString;

    if (size() > 0)
    {
	while (*cp)
	{
	    if (islower(*cp))
		*temp = toupper(*cp);
	    else
		*temp = *cp;
	    temp++;
	    cp++;
	}
	*temp = '\0';

	rVal = start;
	delete start;
    }
    return rVal;
}

String String::operator+ ( const String& toadd ) const
{
    int l1 = size();
    int l2 = toadd.size();
    String stemp;

    if (l1 > 0)				/* ie this is not the NullString */
    {
	if (l2 > 0)
	{
	    char *temp = new char [l1+l2+1];
	    ::strcpy (temp, sp->s);
	    ::strcat (temp, toadd.sp->s);
	    stemp = String(temp);
	    delete [] temp;
	}
	else
	    stemp = *this;
    }
    else
	stemp = toadd;

    return stemp;
}

String String::operator+ ( String *sPtr ) const
{
    if (sPtr)
	return operator+(*sPtr);
    else
	return *this;
}

String& String::operator+= ( const String& olds )
{
    *this = *this + olds;
    return *this;
}

String& String::operator= ( const String& olds )
{
    /* Quick check for S = S */

    if (this == &olds)
	return *this;

    if (sp && (--sp->ref == 0))
    {
	delete [] sp->s;
	delete sp;
    }
    sp = olds.sp;
    if (sp)
	sp->ref++;

    return *this;
}

String& String::operator= ( const char *cstring )
{
    register int len = (cstring != 0 ? strlen(cstring)+1 : 1);

    if (sp && (--sp->ref == 0))
    {
	delete [] sp->s;
	delete sp;
    }    

    sp = new string;
    sp->s = new char[len];
    if (cstring != 0)
	strcpy (sp->s, cstring);
    sp->s[len-1] = '\0';
    sp->ref = 1;
    sp->slen = len-1;

    return *this;
}

/*
 * Substring operation
 */

String String::operator() ( int start, int len ) const
{
    String temp;
    int thissize = sp->slen;
    int copylen;

    /* Sanity checks */

    if (start < 0)
	start = 0;

    if (len < 0)
	len = 0;

    /* Zero length - return null string */

    if ((start == 0) && (len == 0))
	return NullString;

    /* Off end - return null string */

    if (start >= thissize)
	return NullString;

    if ((len == 0) || ((start + len) > thissize))
	copylen = thissize - start;
    else
	copylen = len;
    
    {
	register char *cp = sp->s;
	register char *buff = new char[copylen+1];
	register char *bp;
	register int i;

	bp = buff;
	cp += start;
	for (i = 0; i < copylen; i++)
	    *bp++ = *cp++;
	*bp++ = 0;
	temp = buff;
	delete [] buff;
    }
    
    return temp;	
}

int String::operator== ( const String& other ) const
{
    if (sp && sp->s && other.sp && other.sp->s)
	return (strcmp(sp->s, other.sp->s) == 0);
    return 0;
}

int String::operator== ( const char *other ) const
{
    if (sp && sp->s && other)
	return (strcmp(sp->s, other) == 0);
    return 0;
}

int String::operator!= ( const String& other ) const
{
    if (sp && sp->s && other.sp && other.sp->s)
	return (strcmp(sp->s, other.sp->s) != 0);
    return 1;
}

int String::operator!= ( const char *other ) const
{
    if (sp && sp->s && other)
	return (strcmp(sp->s, other) != 0);
    return 1;
}

#ifdef NO_INLINES
#  define STRING_CC_
#  include "String.n"
#  undef STRING_CC_
#endif
