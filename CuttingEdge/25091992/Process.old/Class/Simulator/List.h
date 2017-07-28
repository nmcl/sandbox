#ifndef List_
#define List_

class Transaction;
class OA;

// class to handle the management of Transactions linked list
class TList
{
private:
    TList(Transaction*);
    TList *next;
public:
    ~TList();

    static void Clear(TList*&);
    static boolean Empty(TList*);
    static void Add(TList*&, Transaction*);
    static void Delete(TList*&, TList*&);

    TList* Next();
    Transaction *Xaction;
};

// class to handle the management of ReplicatedObject linked list
class OList
{
private:
    OList(OA*);
    OList *next;
public:
    ~OList();

    static void Clear(OList*&);
    static boolean Empty(OList*);
    static void Add(OList*&, OA*);
    static void Delete(OList*&, OList*&);

    OList* Next();
    OA *ORef;
};

#endif
