#include <iostream>

#ifndef _Common_
#define _Common_

#define addr_r unsigned int
#define opcode unsigned int

#define READ 0
#define WRITE 1

#define boolean bool

enum direction { ON_BUS, OFF_BUS };

struct data_buffer
{
    int value;
    data_buffer *next_buffer;
};

#endif
