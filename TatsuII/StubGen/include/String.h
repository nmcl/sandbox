/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: String.h,v 1.1 1997/06/09 19:52:17 nmcl Exp $
 */

#ifndef STRING_H_
#define STRING_H_

/*
 *
 * String class. Based on one in THE book. Functions that return String&
 * generally return references to NEWLY allocated strings except those 
 * operators that deal with assignment.
 *
 */

#include <string.h>

#include <iostream>

using namespace std;

/*
 * Since we redo index and rindex for Strings make sure that if these
 * are macros they do not cause confusion here
 */

#ifdef index
#   undef index
#endif

#ifdef rindex
#   undef rindex
#endif

class String
{

public:
    /* String constructors and destructors */

    String ();
    String (const char*);
    String (const unsigned char*);
    String (const String&);
    String (const String&, int);
    ~String ();

    /* Public functions */
    
    String index (const char) const;
    void printout (ostream&) const;
    String rindex (const char) const;
    String removeChars (const String &) const;
    int size () const;
    String strpbrk (const char *) const;
    String touppercase () const;

    /* Public operators */

    String operator+ (const String&) const;
    String operator+ (String *) const;

    String& operator+= (const String&);
    String& operator= (const String&);
    String& operator= (const char *);
    String operator() (int, int) const; /* substring */

    int operator== (const String&) const;
    int operator== (const char *) const;
    int operator!= (const String&) const;
    int operator!= (const char *) const;

    /* Conversion operators */

    operator char *() const;
    operator const char *() const;

private:
    struct string
    {
	char *s;
	int ref;
	unsigned slen;
    };
    string *sp;

};

extern String NullString;

#include "String.n"

#endif
