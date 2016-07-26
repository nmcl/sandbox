/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Defs.h,v 1.3 1996/11/27 11:51:27 snowman Exp $
 */

#ifndef DEFS_H_
#define DEFS_H_

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

/*
 * Some external definitions of useful functions.
 */

extern void reportf (char* s, char* a = "", int b = 0, int c = 0, int d = 0, int e = 0, int f = 0);
extern void GetLine( int Connection, char* Line, int Length );
extern void Send( int Connection, char* Line );
extern void Receive( int Connection, char* Line );
extern void CreateActionFault( char* Line, char* Why, Boolean isFatal );
extern void Log( char* Req, int ReqLen, char* Reply, int ReplyLen );
extern void NewRec( char* Command, char* Reply );
extern void CloseRec( char* Command, char* Reply );
extern void SaveRec( char* Command, char* Reply );
extern void DeleteRec( char* Command, char* Reply );
extern void OpenRec( char* Command, char* Reply );
extern void FindRec( char* Command, char* Reply );
extern void ReadRec( char* Command, char* Reply );
extern int CheckToken( char* Token );
extern int CheckVersion( char* VersionLine, char* Reply );
extern void InitSKUtils();
extern int ResolveKey( char* UserID, char* UserKeyHex );
extern void desinit(char* key);
extern char *HexStringToBytes( char* HexString, char* Bytes, int Length );
extern void StartTime();
extern void EndTime();
extern int DBClose( char* StudNum );
extern int DBFetch( char* StudNumb, char* Reply, Boolean Lock );
#ifdef ARJUNA
extern int DBFind( char* StudNumb, char* Reply );
extern int openDBM ();
extern int closeDBM ();
#endif
extern void ApplySecurity( char* Message );
extern char *BytesToHexString( char* Bytes, char* HexString, int Length );
extern void endes(char* inblock, char* outblock);
extern void xor(char* a, char* b);
extern void dedes(char* inblock, char* outblock);
extern int GetLock( char* File, int Style );
extern int ReleaseLock( int fd );
extern int DBDel( char* StudNumb );
extern int ViolateSec( char* Command );
extern void CreateOpenRecFail( char* String, char* Message );
extern int DBNew( char* StudNumb, char* Reply );
extern int DBStore( char* NewData );
extern int CheckMask( char* InputString );
extern int ChangeLock( int fd, int Style );
extern void CalcCheck( int StudentNumber, char* Result );
extern void ApplyMask( char* Record, char MaskChar );
extern int GetUser( char* UserID, struct DBEntry* UserRecord );
extern void CreateBlankRec (char* Record);
extern Boolean IsRegistration();
extern int seclogin( int connection, char* UserID );
extern void CreateToken( char* TokenText );
extern void ConfigNet(int Port);
extern char *EncodeBlock( char* In, char* Out, int Length );
extern char *DecodeBlock( char* In, char* Out, int Length );
extern void strongk(char* in, char* out);
extern void AddCheckDigit( char* StudentNumber );

#endif
