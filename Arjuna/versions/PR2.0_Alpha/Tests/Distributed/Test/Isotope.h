#ifndef ISOTOPE_H_
#define ISOTOPE_H_

#include <Common/Boolean.h>
#include <Arjuna/LockMan.h>
#include <Arjuna/ObjState.h>
#include <RPC/ArjServers/ArjName.h>

class Isotope : public LockManager
{
  private:

    int    atomic_number;
    int    mass_number;
    double half_life;
    char   name[80];
    ArjunaName* AN;

  public:

    // (de)constructors

    Isotope(char*, int&);
    Isotope(ArjunaName*, int&);
    ~Isotope();

    // Proper object methods

    Boolean set_atomic_number(int);
    Boolean set_mass_number(int);
    Boolean set_half_life(double);
    Boolean set_name(char *);
    int get_atomic_number();
    int get_mass_number();
    double get_half_life();
    char *get_name();
    // Arjuna methods

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;

}; // Isotope

#endif // ISOTOPE

