#ifndef ATOMICINT_H_
#define ATOMICINT_H_


#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif


class AtomicInt : public LockManager
{
public:
    AtomicInt (int&);
    AtomicInt (int&, const Uid&);
    ~AtomicInt ();

    Boolean set (int);
    Boolean get (int&);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    int value;
};

#endif
    

