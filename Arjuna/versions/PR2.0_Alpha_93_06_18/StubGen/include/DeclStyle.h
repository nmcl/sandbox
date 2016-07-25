/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclStyle.h,v 1.5 1993/11/02 12:36:43 ngdp Exp $
 */

#ifndef DECLSTYLE_H_
#define DECLSTYLE_H_

/*
 *
 * Enum describing which particular style of type declaration to produce
 *
 */

enum DeclStyle { CLIENT_DECL = 1, SERVER_DECL = 2, 
		 SERVERIMPLEMENTATION_DECL = 4,
		 NORMAL_DECL = 8 };

#endif
