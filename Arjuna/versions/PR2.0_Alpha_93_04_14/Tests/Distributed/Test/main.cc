#include "Isotope.h"

#include <Configure.h>

#include <iostream.h>
#include <string.h>
#include <ctype.h>

#include <Common/Uid.h>
#include <Arjuna/AtomicA.h>

int add_isotope(void)
{
  AtomicAction A, B;
  Boolean lock;
  int res = 0;

  int atomic_number;
  int mass_number;
  double half_life;
  char name[80];
  char buffer[80];

  //Isotope* x = new Isotope(res);
  //Uid u = x->get_uid();

  A.Begin();
  cout << "A.Begin : Started\n";

  cout << "Enter isotope's atomic number: ";
  cin >> atomic_number;
  cout << "Enter isotope's mass number: ";
  cin >> mass_number;
  cout << "Enter isotope's half-life in minutes: ";
  cin >> half_life;
  cout << "Enter the name of this isotope: ";
  cin >> name;
  // Make a name from the Name and Mass
  sprintf(buffer, "%s-%d", name, mass_number);
  cout << "Object named `" << buffer << "' being created" <<endl;
  Isotope *x = new Isotope(buffer, res);

  if(res < 0) {
    cout << "No such object\n";
    return(0);
  } // if

  cout << "\nSetting values ...\n";
  Uid u = x->get_uid();
  cout << "UID: " << u << endl;
  lock = x->set_atomic_number(atomic_number);
  if (!lock) {
    cout << "Could not acquire lock on atomic number\n";
    A.Abort();
  } else {
    lock = x->set_mass_number(mass_number);
    if (!lock) {
      cout << "Could not acquire lock on mass number\n";
      A.Abort();
    } else {
      lock = x->set_half_life(half_life);
      if (!lock) {
        cout << "Could not acquire lock on half-life\n";
        A.Abort();
      } else {
        lock = x->set_name(name);
        if (!lock) {
          cout << "Could not acquire lock on isotope name\n";
          A.Abort();
        } else {
          // Commit data
          if(A.End() != COMMITTED) {
            cout << "Commit failed\n";
          } else {
            cout << "Commit completed successfully\n";
          } // if
        } // if
      } // if
    } // if
  } // if

  delete x;
  cout << "Add done.\n";
  return 0;
}

int get_isotope(void)
{
  char name[80];
  int  lock;
  int res;
  double val;
  AtomicAction A;

  cout << "Enter the name of this isotope: ";
  cin >> name;
  cout << "Getting object `" << name << "'" << endl;
  A.Begin();
  ArjunaName AN(name);
  Isotope *x = new Isotope(&AN, res);

  if(res < 0) {
    cout << "No such object\n";
    return(0);
  } // if
  cout << "\nGetting values ...\n";
  lock = x->get_atomic_number();
  cout << "atomic number: " << lock << "\n";
  lock = x->get_mass_number();
  cout << "mass number: " << lock << "\n";
  val = x->get_half_life();
  cout << "half-life: " << val << "\n";
  strcpy(name, x->get_name());
  cout << "Name: " << name << "\n";
  delete x;
  cout << "Find done.\n";
  A.End();
  return 0;

}

int main(int argc, char **argv)
{
  int choice;

  do {
    cout << "Isotopes Database" << endl;
    cout << "-----------------" << endl << endl;
    cout << "1 - Add isotope" << endl;
    cout << "2 - View isotope" << endl;
    cout << "3 - Quit" << endl;
    cout << endl << "Enter choice: ";
    cin >> choice;

    switch(choice) {
      case 1:
        add_isotope();
        break;
      case 2:
        get_isotope();
        break;
      case 3:
        break;
      default:
        break;
    } // switch
  } while(choice != 3);
}
