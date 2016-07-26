#ifndef LOCAL_H_
#define LOCAL_H_

#ifndef LOCALCONFIG_H_
#  include <localconfig.h>
#endif

#define DEBUG_USER "Debug"

#define MAXAUTHTIME 60
#define MAXVERSIONTIME 90
#define LOWERLIMIT 60
#define UPPERLIMIT 60
#define NOYOUNGLIMIT 3

#define OK 0
#define NOT_OK -1

struct IndexDBEntry
{
    long Position;
    int Status;
    int Modified;
};

#define MAXBUFF 4096
#define MAXFINDRECS 500
#define YEARS_70 2208988800
#define USERNAMELEN 8
#define DEPARTMENTLEN 8
#define RECSIZE 1024
#define STUDENTNUMBERSIZE 9
#define STUDATASIZE 1015
#define EOLN "\r\n"

#define CREATENEWPATTERN "000000000"
#define NORECORD "---------"
#define ONEYEARSECS 31536000
#define NEWACCYEAR "0701"

#endif
