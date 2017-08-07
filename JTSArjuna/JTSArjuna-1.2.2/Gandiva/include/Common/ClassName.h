/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClassName.h,v 1.2 1996/01/02 13:12:32 nsmw Exp $
 */

#ifndef COMMON_CLASSNAME_H_
#define COMMON_CLASSNAME_H_

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

class ostream;
class Buffer;


class ClassName
{
public:
    ClassName (const char* className);
    ClassName (const ClassName&);
    virtual ~ClassName ();

    ClassName& operator=  (const ClassName& className);
    Boolean    operator== (const ClassName& className) const;
    Boolean    operator!= (const ClassName& className) const;

    char* stringForm () const;

    static const ClassName& invalid ();
    
    Boolean pack   (Buffer&) const;
    Boolean unpack (Buffer&);
    
    ostream& print (ostream& strm) const;

private:
    char* _className;

    static ClassName* _invalid;
};

extern ostream& operator<< (ostream& strm, const ClassName& className);

#endif
