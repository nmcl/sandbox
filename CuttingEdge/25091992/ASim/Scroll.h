#include <InterViews/patch.h>
#include <InterViews/listener.h>
#include <InterViews/handler.h>
#include <InterViews/glyph.h>
#include <InterViews/color.h>
#include <InterViews/font.h>
#include <InterViews/world.h>
#include <InterViews/box.h>
#include <InterViews/center.h>
#include <InterViews/glue.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

// This class provides a scrolling window with a program counter indicator.

class ScrollingTextPC : public Listener, public Handler
{
    Patch* me;  // The body of the window to be displayed
    char *text; // The text to scroll
    points *page_top, *page_ptrs;
    int pc_pos; // position of program counter
    int lines, skip_lines;
    const Color *fg; // The color to display the text in
    const Font *f;   // The font to display the text in
    
    void alter_text();   // Changes all \n in the text to \0
    void redraw_pc();    // Redraws the position of the program counter
    void redraw_area();  // Redraws the contents of the text area

public:
    ScrollingTextPC(char*, Patch*, World*);
    virtual ~ScrollingTextPC();

    void update_text(int, char*);  // Used to update portions of the text
    void goto_line(int);          // Used to make the program counter go to a specific line
    virtual void event(Event&);
};

