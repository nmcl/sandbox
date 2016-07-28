/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclStyle.h,v 1.4 1994/09/14 15:05:42 ngdp Exp $
 */

#ifndef DECLSTYLE_H_
#define DECLSTYLE_H_

/*
 *
 * Enums describing which particular style of type declaration to produce
 *
 */

enum DeclStyle { CLIENT_DECL = 1, SERVER_DECL = 2, 
		 SERVERIMPLEMENTATION_DECL = 4,
		 NORMAL_DECL = 8, IDL_DECL = 16, SHADOWS_DECL = 32 };
#endif






