#ifndef IMPLEMENTATION_PIDFACTORYIMPLELIST_H_
#define IMPLEMENTATION_PIDFACTORYIMPLELIST_H_

class PIDFactory_Init;
class FactoryListElement;

class PIDFactoryImpleList
{
public:
    static void add    (PIDFactory_Init*);
    static void remove (PIDFactory_Init*);

    static PIDFactory_Init* find (const char* name);

protected:
    PIDFactoryImpleList ();
    ~PIDFactoryImpleList ();
    
private:
    static FactoryListElement* _headOfList;
};

#endif
