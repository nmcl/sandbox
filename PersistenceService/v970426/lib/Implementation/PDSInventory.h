#ifndef IMPLEMENTATION_PDSINVENTORY_H_
#define IMPLEMENTATION_PDSINVENTORY_H_

#ifdef HAVE_CH_EXTENSION
#  include <Interface/CosPersistencePDS.ch>
#else
#  include <Interface/CosPersistencePDS.h>
#endif

class PDSListElement;

class PDSInventory
{
public:
    static void addCreateVoid (CosPersistencePDS::PDS_ptr (*create)(), const char* name);

    static CosPersistencePDS::PDS_ptr createVoid (const char* string);

protected:
    PDSInventory ();
    ~PDSInventory ();
    
private:
    static PDSListElement* _headOfList;
};

#endif
