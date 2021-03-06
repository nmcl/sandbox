#ifndef COMMON_EXCEPTIONS_H_
#define COMMON_EXCEPTIONS_H_

#define PDS_BASE          20000
#define ALREADY_CONNECTED PDS_BASE+1
#define NOT_CONNECTED     PDS_BASE+2
#define NO_OBJECTSTORE    PDS_BASE+3
#define OBJECT_UNPACK     PDS_BASE+4
#define OBJECT_PACK       PDS_BASE+5
#define READ_STATE        PDS_BASE+6
#define WRITE_STATE       PDS_BASE+7
#define OBJECT_DELETED    PDS_BASE+8

#define PO_BASE           30000
#define IS_CONNECTED      PO_BASE+1

#endif

