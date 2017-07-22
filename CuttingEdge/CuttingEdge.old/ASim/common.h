#ifndef _COMMON_
#define _COMMON_
struct points
{
    char *ptr;
    points *next_ptr, *last_ptr;
};

#define LINES 15
// This is a blank box within a window
#define Blank TBBox(vspace, vspace, vspace, vspace, vspace, vspace, vspace, vspace, vspace, vspace)

// This is a single column blank window
#define Blank_Window TBBox(new Blank, new Blank, new Blank)

// This is a double column blank window
#define Two_Columns LRBox(new Blank_Window, new Blank_Window)

#define blank " "
#define Blank_Line Patch(new VCenter(Message(f, fg, blank), 1.0))
#define PC_Ptr Patch(new VCenter(Message(f, fg, "->"), 1.0))
#define blank_space "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\0"
#endif

extern "C"
{
    void bcopy(char*, char*, int);
    char *realloc(char*, int);
}

