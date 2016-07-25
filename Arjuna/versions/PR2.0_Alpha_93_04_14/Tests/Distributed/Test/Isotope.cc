#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define ISOTOPE_SERVER

#include "Isotope.h"

#include <iostream.h>
#include <Arjuna/AtomicA.h>
#include <string.h>

Isotope::Isotope(char *object_name, int& res) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;

    AN = new ArjunaName; // should not declare on the stack as this is an atomic object

    A.Begin();
    if (setlock(new Lock(WRITE), 0) == GRANTED) {
        AN->Register(object_name, this);
        atomic_number = -1;
	mass_number = -1;
        half_life = -1.0;
        strcpy(name, object_name);
	A.End();
	res = 0;
    } else {
	A.Abort();
	res = -1;
    } // if
} // Isotope

Isotope::Isotope(ArjunaName* AN, int& res) : LockManager(AN->GetRefObjUid())
{
    AN = 0;
    res = 0;
} // Isotope

Isotope::~Isotope()
{
  terminate();

  if (AN)
      delete AN;

} // ~Isotope

Boolean Isotope::set_atomic_number(int new_value)
{
    AtomicAction B;

    B.Begin();
    if (setlock(new Lock(WRITE), 0) == GRANTED) {
	atomic_number = new_value;
	B.End();
	return (TRUE);
    } else {
	B.Abort();
	return (FALSE);
    } // if
} // set_atomic_number

Boolean Isotope::set_mass_number(int new_value)
{
    AtomicAction B;

    B.Begin();
    if (setlock(new Lock(WRITE), 0) == GRANTED) {
        mass_number = new_value;
        B.End();
        return (TRUE);
    } else {
        B.Abort();
        return (FALSE);
    } // if
} // set_mass_number

Boolean Isotope::set_half_life(double new_value)
{
    AtomicAction B;

    B.Begin();
    if (setlock(new Lock(WRITE), 0) == GRANTED) {
        half_life = new_value;
        B.End();
        return (TRUE);
    } else {
        B.Abort();
        return (FALSE);
    } // if
} // set_half_life

Boolean Isotope::set_name(char *new_value)
{
    AtomicAction B;

    B.Begin();
    if (setlock(new Lock(WRITE), 0) == GRANTED) {
        strcpy(name, new_value);
        B.End();
        return (TRUE);
    } else {
        B.Abort();
        return (FALSE);
    } // if
} // set_name

int Isotope::get_atomic_number()
{
    AtomicAction C;

    C.Begin();
    if (setlock(new Lock(READ), 0) == GRANTED) {
	C.End();
	return (atomic_number);
    } else {
	C.Abort();
	return (-1);
    } // if
} // get_atomic_number

int Isotope::get_mass_number()
{
    AtomicAction C;

    C.Begin();
    if (setlock(new Lock(READ), 0) == GRANTED) {
        C.End();
        return (mass_number);
    } else {
        C.Abort();
        return (-1);
    } // if
} // get_mass_number

double Isotope::get_half_life()
{
    AtomicAction C;

    C.Begin();
    if (setlock(new Lock(READ), 0) == GRANTED) {
        C.End();
        return (half_life);
    } else {
        C.Abort();
        return (-1.0);
    } // if
} // get_half_life

char *Isotope::get_name()
{
    AtomicAction C;

    C.Begin();
    if (setlock(new Lock(READ), 0) == GRANTED) {
        C.End();
        return (name);
    } else {
        C.Abort();
        return (NULL);
    } // if
} // get_name

Boolean Isotope::save_state(ObjectState& os, ObjectType)
{
  Boolean res;
  
  if(res = os.pack(atomic_number)) {
    if(res = os.pack(mass_number)) {
      if(res = os.pack(half_life)) {
        res= os.pack(name);
      } // if
    } // if
  } // if
  return(res);
} // save_state

Boolean Isotope::restore_state(ObjectState& os, ObjectType)
{
  Boolean res;
  
  if(res = os.unpack(atomic_number)) {
    if(res = os.unpack(mass_number)) {
      if(res = os.unpack(half_life)) {
        res = os.unpack(name);
      } // if
    } // if
  } // if
  return(res);
} // restore_state

const TypeName Isotope::type() const
{
    return "/StateManager/LockManager/Isotope";
} // type
