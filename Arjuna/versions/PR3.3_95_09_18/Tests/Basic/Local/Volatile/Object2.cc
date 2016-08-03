/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Object2.cc,v 1.2 1994/03/18 15:40:48 n048z Exp $
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

#include "Object2.h"

Object2::Object2(ObjectType t, Uid& u, int& res) 
: i(0), j(0),
LockManager(t, TRUE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Object2::Object2(" << t
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

Object2::Object2(ObjectType t, int newi, int newj, Uid& u, int& res) 
: i(0),j(0), 
LockManager(t, TRUE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Object2::Object2(" << t
		 << get_uid() << ", int&)" << endl;
#endif
    res = 0;
    u = get_uid();
    AtomicAction AA;
    AA.Begin();
    if (setlock(new Lock(WRITE)) == GRANTED) {
	res = 1;
	i = newi;
	j = newj;
    }
    if (res) {
	if (AA.End() != COMMITTED)
	res = 0;
    }
    else
	AA.Abort();
}

Object2::Object2(Uid& u, ObjectType t, int& res)
: i(0),j(0),
LockManager(u, t)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Object2::Object2(" << u << ", " << t << ", " << "int&)" << endl;
#endif    
    res = 1;
}

Object2::~Object2()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Object2::~Object2()" << endl;
#endif
    terminate();
}

Boolean Object2::change(int newi, int newj, int& oldi, int& oldj)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Object2::change(" << newi << ", " << newj
		 << ", int&, int&)" << endl;
#endif
    Boolean res = 0;
    AtomicAction AA;
    AA.Begin();
    if (setlock(new Lock(WRITE)) == GRANTED) {
	res = 1;
	oldi = i;
	i = newi;
	oldj = j;
	j = newj;
    }
    if (res) {
	if (AA.End() != COMMITTED)
	    res = 0;
    }
    else
	AA.Abort();
    return res;
}

Boolean Object2::get(int& curri, int& currj)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Object2::get(int&, int&)" << endl;
#endif
    Boolean res = 0;
    AtomicAction AA;
    AA.Begin();
    if (setlock(new Lock(WRITE)) == GRANTED) {
	res = 1;
	curri = i;
	currj = j;
    }
    if (res) {
	if (AA.End() != COMMITTED)
	    res = 0;
    }
    else
	AA.Abort();
    return res;
}

Boolean Object2::save_state(ObjectState& os, ObjectType t)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << VIS_PUBLIC << FAC_USER1;
    debug_stream << "Object2::save_state( , " << t << ")" << endl;
#endif
    Boolean res = os.pack(i) && os.pack(j);
    return res;
}

Boolean Object2::restore_state(ObjectState& os, ObjectType t)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << VIS_PUBLIC << FAC_USER1;
    debug_stream << "Object2::restore_state( , " << t << ")" << endl;
#endif
    Boolean res = os.unpack(i) && os.unpack(j);
    return res;
}

const TypeName Object2::type() const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << VIS_PUBLIC << FAC_USER1;
    debug_stream << "Object2::type()" << endl;
#endif
    return "/StateManager/LockManager/Object2";
}

