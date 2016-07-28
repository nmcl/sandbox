/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: MenuItemDispatch.cc,v 1.1 1993/11/11 15:18:45 nmcl Exp $
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
