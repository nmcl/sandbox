/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: MenuItemDispatch.h,v 1.2 1994/10/31 12:07:29 nsmw Exp $
 */

#ifndef MENUITEMDISPATCH_H_
#define MENUITEMDISPATCH_H_

class MenuItemDispatch;

#include <IV-2_6/InterViews/menu.h>

#include "TheatreDispatch.h"

enum DispatchOper { new_oper, open_oper, close_oper, blockbooking_begin_oper,
		    blockbooking_commit_oper, blockbooking_abort_oper,
		    subblockbooking_begin_oper, subblockbooking_commit_oper,
		    subblockbooking_abort_oper, log_open_oper,
		    log_close_oper, reserve_oper, unreserve_oper,
		    redisplay_oper, redisplay_all_oper, nop_oper, quit_oper };

class MenuItemDispatch :public MenuItem
{
public:
    MenuItemDispatch(const char*, TheatreDispatch*, enum DispatchOper = nop_oper);
    ~MenuItemDispatch();

    virtual void Do();
private:
    enum DispatchOper oper_num;
    TheatreDispatch*  TheatreDisp;
};

#endif
