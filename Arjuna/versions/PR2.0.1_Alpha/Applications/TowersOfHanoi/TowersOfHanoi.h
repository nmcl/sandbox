/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: TowersOfHanoi.h,v 1.1 1993/11/03 12:25:42 nmcl Exp $
 */

#ifndef TOWERSOFHANOI_H_
#define TOWERSOFHANOI_H_

#ifdef STUB
#  pragma @NoRename
#endif

#define maxDisk 6

// @NoRemote, @UserMarshall
class Uid;

extern char* Pin1Host;
extern Uid*  Pin1Uid;
extern char* Pin2Host;
extern Uid*  Pin2Uid;
extern char* Pin3Host;
extern Uid*  Pin3Uid;
extern char* Pin4Host;
extern Uid*  Pin4Uid;
extern char* WSHost;
extern Uid*  WSUid;
extern int   TowersNew;

#endif
