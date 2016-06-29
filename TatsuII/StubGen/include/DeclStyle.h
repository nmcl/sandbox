/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclStyle.h,v 1.3 1997/06/12 20:06:18 nmcl Exp $
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
		 NORMAL_DECL = 8, IDL_DECL = 16, SHADOWS_DECL = 32,
                 JAVA_DECL = 64, ARJUNAII_CLIENT_DECL = 128,
                 ARJUNAII_INTF_DECL = 256, ARJUNAII_SERVER_DECL = 512 };
#endif






