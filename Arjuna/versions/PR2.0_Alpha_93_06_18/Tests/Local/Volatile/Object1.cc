/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Object1.cc,v 1.3 1993/07/19 10:59:14 n048z Exp $
 */

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef DEBUG_H_
#include <Common/Debug.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#include "Object1.h"

Object1::Object1(ObjectType t, Uid& u, int& res) 
: i(0), 
LockManager(t)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Object1::Object1(" << t
		 << get_uid() << ", int&)" << endl;
#endif
    res = 0;
    u = get_uid();
    AtomicAction AA;
    AA.Begin();
    if (setlock(new Lock(WRITE)) == GRANTED) {
	res = 1;
    }
    if (res) {
	if (AA.End() != COMMITTED)
	res = 0;
    }
    else
	AA.Abort();
}

Object1::Object1(ObjectType t, int newi, Uid& u, int& res) 
: i(0), 
LockManager(t)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Object1::Object1(" << t
		 << get_uid() << ", int&)" << endl;
#endif
    res = 0;
    u = get_uid();
    AtomicAction AA;
    AA.Begin();
    if (setlock(new Lock(WRITE)) == GRANTED) {
	res = 1;
	i = newi;
    }
    if (res) {
	if (AA.End() != COMMITTED)
	res = 0;
    }
    else
	AA.Abort();
}

Object1::Object1(Uid& u, ObjectType t, int& res)
: i(0),
LockManager(u, t)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Object1::Object1(" << u << ", " << t
		 << ", " << "int&)" << endl;
#endif    
    res = 1;
}

Object1::~Object1()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Object1::~Object1()" << endl;
#endif
    terminate();
}

Boolean Object1::change(int newi, int& oldi)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Object1::change(" << newi << ", int&)" << endl;
#endif
    Boolean res = 0;
    AtomicAction AA;
    AA.Begin();
    if (setlock(new Lock(WRITE)) == GRANTED) {
	res = 1;
	oldi = i;
	i = newi;
    }
    if (res) {
	if (AA.End() != COMMITTED)
	    res = 0;
    }
    else
	AA.Abort();
    return res;
}

Boolean Object1::get(int& curri)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Object1::get(int&)" << endl;
#endif
    Boolean res = 0;
    AtomicAction AA;
    AA.Begin();
    if (setlock(new Lock(READ)) == GRANTED) {
	res = 1;
	curri = i;
    }
    if (res) {
	if (AA.End() != COMMITTED)
	    res = 0;
    }
    else
	AA.Abort();
    return res;
}

Boolean Object1::save_state(ObjectState& os, ObjectType t)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << VIS_PUBLIC << FAC_USER1;
    debug_stream << "Object1::save_state( , " << t << ")" << endl;
#endif
    return os.pack(i);
}

Boolean Object1::restore_state(ObjectState& os, ObjectType t)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << VIS_PUBLIC << FAC_USER1;
    debug_stream << "Object1::restore_state( , " << t << ")" << endl;
#endif
    return os.unpack(i);
}

const TypeName Object1::type() const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << VIS_PUBLIC << FAC_USER1;
    debug_stream << "Object1::type()" << endl;
#endif
    return "/StateManager/LockManager/Object1";
}

