/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: MenuItemDispatch.cc,v 1.2 1993/03/18 14:03:16 nmcl Exp $
 */

#include "TheatreDispatch.h"

#include "MenuItemDispatch.h"

MenuItemDispatch::MenuItemDispatch(const char* str, TheatreDispatch* ThDisp,
				   enum DispatchOper oper_n) : MenuItem(str)
{
    oper_num = oper_n;
    TheatreDisp = ThDisp;
}

MenuItemDispatch::~MenuItemDispatch()
{
}

void MenuItemDispatch::Do()
{
    TheatreDisp->MenuDo(oper_num);
}
