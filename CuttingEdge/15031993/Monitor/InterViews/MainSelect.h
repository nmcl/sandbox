#ifndef MAINSELECT_H_
#define MAINSELECT_H_

#include <InterViews/action.h>
#include <InterViews/label.h>
#include <InterViews/window.h>
#include <IV-look/menu.h>
#include <IV-look/field.h>
#include <IV-look/dialogs.h>

#ifndef COMMON_H_
#include "Common.h"
#endif


class buttonCheck
{
public:
    static Boolean inUse;
    static Boolean shouldDelete;
    static const char* modifyName;
    
private:
    buttonCheck () {};
    ~buttonCheck () {};
};


class Add : public Action
{
public:
    Add ();
    ~Add ();

    static ApplicationWindow *myWindow;
    void addWindow (ApplicationWindow*);
    
    virtual void execute ();

private:
    ApplicationWindow *mainWindow;
};


class Delete : public Action
{
public:
    Delete ();
    ~Delete ();

    static ApplicationWindow *myWindow;
    static int toRemove;
    
    virtual void execute ();
};


class Modify : public Action
{
public:
    Modify ();
    ~Modify ();
    
    static ApplicationWindow *myWindow;
    
    virtual void execute ();
};


class Output : public Action
{
public:
    Output ();
    ~Output ();
    
    virtual void execute ();
};


class addNode : public Action
{
public:
    addNode (FieldEditor*);
    ~addNode ();
    
    virtual void execute ();

private:
    FieldEditor* fe;
};


class deleteNode : public Action
{
public:
    deleteNode ();
    ~deleteNode ();
    
    virtual void execute ();
};


class modifyNode : public Action
{
public:
    modifyNode (FieldEditor*);
    ~modifyNode ();
    
    virtual void execute ();
    
private:
    FieldEditor* fe;
};




#ifdef _INIT_
     ApplicationWindow* Add::myWindow = 0;
     ApplicationWindow* Delete::myWindow = 0;
     ApplicationWindow* Modify::myWindow = 0;
     int Delete::toRemove = -1;
     Boolean buttonCheck::inUse = FALSE;
     Boolean buttonCheck::shouldDelete = FALSE;
     const char* buttonCheck::modifyName = 0;
#endif

#endif

