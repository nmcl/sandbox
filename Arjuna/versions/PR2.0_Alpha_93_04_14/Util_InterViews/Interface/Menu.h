/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $id:$
 */

#ifndef MENU_H_
#define MENU_H_

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#include <InterViews/action.h>
#include <InterViews/background.h>
#include <InterViews/patch.h>
#include <InterViews/glyph.h>
#include <InterViews/center.h>
#include <InterViews/world.h>
#include <InterViews/box.h>
#include <InterViews/font.h>
#include <InterViews/color.h>
#include <InterViews/label.h>
#include <InterViews/margin.h>
#include <InterViews/glue.h>
#include <InterViews/window.h>
#include <IV-look/field.h>
#include <IV-look/menu.h>
#include <IV-look/telltale.h>

#include "Interface_stub.h"

class Selector;
class ScrollingText;

class foo : public Action
{
public:
    foo ();
    ~foo  ();

    virtual void execute ();
};

class Commit : public Action
{
public:
    Commit (void (*func)());
    Commit (void (*func)(Selector*), Selector*);
    ~Commit ();

    virtual void execute ();

private:
    void (*x) ();
    void (*y) (Selector*);
    Selector *me;
};

class Entry : public Action
{
public:
    Entry (Interface*);
    ~Entry ();

    virtual void execute ();
    static void work ();
    static void cancel ();

private:
    char what[BUF_SIZE], where[PATH], machine[NAME];
    void Initialize();
    static State *a;
    static class TopLevelWindow *pw;
    Commit *cmt, *cnl;
    static Commit *fault;
    static FieldEditor *desc;
    static FieldEditor *path_l;
    static FieldEditor *mac_n;
    static FieldEditor *servers;
    static Interface *itf;
    static PopupWindow *error;
};

class Scanner : public Action
{
public:
    Scanner (Interface*);
    ~Scanner ();

    char *contents ();
    void add_body (ScrollingText*);
    virtual void execute ();
    static void cancel ();

private:
    static ScrollingText *stx;
    static Interface *itf;
    static State *a;
    static PopupWindow *error;
    char *buffer;
    static Commit *fault;
};

class Delete : public Action
{
public:
    Delete (Interface*);
    ~Delete ();

    virtual void execute ();
    static void work ();
    static void cancel ();

private:
    static Interface *itf;
    Commit *cmt, *cnl;
    static Commit *fault;
    static class TopLevelWindow *pw;
    static PopupWindow *error;
    static FieldEditor *number;
};

#ifdef SET_
#undef SET_
    class Interface *Scanner::itf = 0;
    class ScrollingText *Scanner::stx = 0;
    class State *Scanner::a = 0;
    class PopupWindow *Scanner::error = 0;
    class Commit *Scanner::fault = 0;

    class TopLevelWindow *Entry::pw = 0;
    class FieldEditor *Entry::desc = 0;
    class FieldEditor *Entry::path_l = 0;
    class FieldEditor *Entry::mac_n = 0;
    class FieldEditor *Entry::servers = 0;
    class PopupWindow *Entry::error = 0;
    class Commit *Entry::fault = 0;
    class State *Entry::a = 0;
    class Interface *Entry::itf = 0;

    class Interface *Delete::itf = 0;
    class TopLevelWindow *Delete::pw = 0;
    class PopupWindow *Delete::error = 0;
    class FieldEditor *Delete::number = 0;
    class Commit *Delete::fault = 0;
#endif
#endif
