#include <string.h>

#include <Arjuna/AtomicAction.h>

#include "ActiveColour.h"

ActiveColour* ActiveColour::head = nil;

ActiveColour* ActiveColour::Create(int& res, Uid obj_uid)
{
    int done = 0;

    res = 1;
    
    ActiveColour* temp = head;
    while ((! done) && (temp != nil))
    {
	if (obj_uid == temp->get_uid())
	    done = 1;
	else
	    temp = temp->next;
    }

    if (temp == nil)
    {
	res        = 0;
	temp       = new ActiveColour(res, obj_uid);
	temp->next = head;
	head       = temp;
    }

    return temp;
}

ActiveColour* ActiveColour::Create(int& res, char* name, ColorIntensity r, 
                                   ColorIntensity g, ColorIntensity b)
{
    ActiveColour* temp = new ActiveColour(res, name, r, g, b);

    temp->next = head;
    head       = temp;

    return temp;
}

void  ActiveColour::Destroy()
{
    ActiveColour* temp = head;

    while (temp != nil)
    {
	temp = head->next;
	delete head;
	head = temp;
    }
}

void ActiveColour::getName(int& res, char*& name)
{
    res = 0;
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        name = colourName;
        res = 1;
    }

    if (res == 1)
    {
	if (A.End() != COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

void ActiveColour::getColour(int& res, ColorIntensity &r_v, 
                             ColorIntensity &g_v, ColorIntensity &b_v)
{
    res = 0;
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        r_v = r;
        g_v = g;
        b_v = b;

        res = 1;
    }

    if (res == 1)
    {
	if (A.End() != COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

void ActiveColour::getRed(int& res, ColorIntensity &r_v)
{
    res = 0;
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        r_v = r;

        res = 1;
    }

    if (res == 1)
    {
	if (A.End() != COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

void ActiveColour::getGreen(int& res, ColorIntensity &g_v)
{
    res = 0;
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        g_v = g;

        res = 1;
    }

    if (res == 1)
    {
	if (A.End() != COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

void ActiveColour::getBlue(int& res, ColorIntensity &b_v)
{
    res = 0;
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        b_v = b;

        res = 1;
    }

    if (res == 1)
    {
	if (A.End() != COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

void ActiveColour::setColour(int& res, ColorIntensity r_v, 
                             ColorIntensity g_v, ColorIntensity b_v)
{
    res = 0;
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
        r = r_v;
        g = g_v;
        b = b_v;

        res = 1;
    }

    if (res == 1)
    {
	if (A.End() != COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

void ActiveColour::setRed(int& res, ColorIntensity r_v)
{
    res = 0;
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
        r = r_v;

        res = 1;
    }

    if (res == 1)
    {
	if (A.End() != COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

void ActiveColour::setGreen(int& res, ColorIntensity g_v)
{
    res = 0;
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
        g = g_v;

        res = 1;
    }

    if (res == 1)
    {
	if (A.End() != COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

void ActiveColour::setBlue(int& res, ColorIntensity b_v)
{
    res = 0;
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
        b = b_v;

        res = 1;
    }

    if (res == 1)
    {
	if (A.End() != COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

bool ActiveColour::save_state(ObjectState& os, ObjectType)
{
    return (bool) (os.pack(r) && os.pack(g) && os.pack(b) && os.pack(colourName, COLOURNAME_SIZE));
}

bool ActiveColour::restore_state(ObjectState& os, ObjectType)
{
    long length = 0;

    return (os.unpack(r) && os.unpack(g) && os.unpack(b) && os.unpack(colourName, length));
}

const TypeName ActiveColour::type() const
{
    return "/StateManager/LockManager/ActiveColour";
}

ActiveColour::ActiveColour(int& res, Uid obj_uid) : LockManager(obj_uid)
{
    res = 1;
}

ActiveColour::ActiveColour(int& res, char* name, ColorIntensity r, 
                           ColorIntensity g, ColorIntensity b) : LockManager(ANDPERSISTENT)
{
    strncpy(colourName, name, strlen(name));
    setColour(res, r, g, b);
}

ActiveColour::~ActiveColour()
{
    terminate();
}
