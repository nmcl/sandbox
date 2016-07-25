/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: OperParamSize.h,v 1.4 1993/09/01 15:26:58 n048z Exp $
 */

#ifndef OPERPARAMSIZE_H_
#define OPERPARAMSIZE_H_

class OperParamSize
{
public:
	OperParamSize();

	void NOPChar0();
	void NOPChar1(char p);
	void NOPChar2(char p[2]);
	void NOPChar4(char p[4]);
	void NOPChar8(char p[8]);
	void NOPChar16(char p[16]);
	void NOPChar32(char p[32]);
	void NOPChar64(char p[64]);
	void NOPChar128(char p[128]);
	void NOPChar256(char p[256]);
	void NOPChar512(char p[512]);
	void NOPChar1024(char p[1024]);
	void NOPChar2048(char p[2048]);
	void NOPChar4096(char p[4096]);
	void NOPChar8192(char p[8192]);
	void NOPChar16384(char p[16384]);
	void NOPChar32768(char p[32768]);
	void NOPChar65536(char p[65536]);
	void NOPChar131072(char p[131072]);
	void NOPChar262144(char p[262144]);

	~OperParamSize();
};

#endif
