/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Uid.h,v 1.3 1996/01/02 13:12:47 nsmw Exp $
 */

#ifndef COMMON_UID_H_
#define COMMON_UID_H_

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

class ostream;

class Buffer;

class Uid
{
public:
    Uid();
    Uid(const Uid &uid);
    Uid(const char *uidString);
    ~Uid();

    Uid& operator= (const Uid &uid);

    Boolean operator== (const Uid &uid) const;
    Boolean operator!= (const Uid &uid) const;
    Boolean operator< (const Uid &uid) const;
    Boolean operator> (const Uid &uid) const;

    unsigned long hash () const;
    
    char *stringForm() const;

    static const Uid &invalid();

    Boolean pack(Buffer &buff) const;
    Boolean unpack(Buffer &buff);

    void print(ostream &ostr) const;

private:
    static Uid* _invalid;

    static unsigned long int _cacheValue0;
    static unsigned long int _cacheValue1;
    static unsigned long int _cacheValue2;
    static unsigned long int _lastValue3;

    unsigned long int _value0;
    unsigned long int _value1;
    unsigned long int _value2;
    unsigned long int _value3;
};

extern ostream &operator<< (ostream &ostr, const Uid &uid);

#endif
