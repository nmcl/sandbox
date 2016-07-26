/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: arjuna.h,v 1.7 1996/11/23 09:58:09 snowman Exp $
 */

#ifndef ARJUNA_H_
#define ARJUNA_H_

#ifndef ARJUNACONFIG_H_
#  include <arjunaconfig.h>
#endif

#define outputFile "/tmp/Regist_output_%d"
#define errorFile "/tmp/Regist_error_%d"
#define counterUid "80f0f763:15b7:3062a1f1:2"
#define serverTimeout 30000
#define serverRetry 9

static const int UidSize = 128;
static const int fileRetry = 5;
static const int fileTimeout = 10;

#endif
