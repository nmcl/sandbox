#include "Scroll.h"
#include "Message.h"

ScrollingTextPC::ScrollingTextPC(char *wds, Patch *body, World *w) : Listener(body, this)
{
    button(true, Event::any);

    me = body;
    lines = 0;
    skip_lines = 0;
    text = wds;
    pc_pos = 0;
    page_ptrs = new points;
    page_top = page_ptrs;

    f = w->font();
    fg = w->foreground();

    alter_text();
}

ScrollingTextPC::~ScrollingTextPC()
{
    free(text);
}

void ScrollingTextPC::alter_text()
{
    char *ptr = text;
    points *marker = page_ptrs, *x;

    marker->ptr = ptr;
    marker->next_ptr = 0;
    marker->last_ptr = 0;
    x = marker;

    ptr = text;
    while (*ptr != '\0')
    {
	if (*ptr == '\n')
	{
	    lines++;
	    *ptr = '\0';
	}
	if ((*ptr == '\0') && (*(ptr+1) == '\0'))
	{
	    lines--;
	    *(ptr+1) = ' ';
	}
	ptr++;

	if (*(ptr -1) == '\0')
	{
	    marker->next_ptr = new points;
	    marker = marker->next_ptr;
	    marker->next_ptr = 0;
	    marker->last_ptr = x;
	    marker->ptr = ptr;
	    x = marker;
	}
    }
    redraw_area();
    redraw_pc();
}

void ScrollingTextPC::redraw_pc()
{
    Patch *blank_l = new Blank_Line;
    Patch *pc_ptr = new PC_Ptr;

    TBBox *b = new TBBox;

    for (int l = 0; l < LINES; l++)
    {
	if (l == pc_pos)
	    b->insert(l, pc_ptr);
	else
	    b->insert(l, blank_l);
    }

    me->replace(0, b);
    me->reallocate();
    me->redraw();
}

void ScrollingTextPC::redraw_area()
{
    TBBox *b = new TBBox;
    points *marker = page_top;

    for (int l = 0; l < LINES; l++)
    {
	Patch *msg = new Patch(new VCenter(Message(f, fg, marker->ptr), 1.0));
	if (marker->next_ptr != 0)
	    marker = marker->next_ptr;
	else
	    break;
	b->insert(l, msg);
    }

    me->replace(1, b);
    me->reallocate();
    me->redraw();
}

void ScrollingTextPC::update_text(int jump_char, char* new_text)
{
    char *ptr = text;
    int ln  = 0;

    for (int k = 0; k < jump_char; k++)
    {
	if (*ptr == '\0')
	    ln++;
	ptr++;
    }

    bcopy(new_text, ptr, strlen(new_text));
    if ((ln >= skip_lines) && (ln < skip_lines + LINES))
	redraw_area();
}

void ScrollingTextPC::goto_line(int line_number)
{
    points *marker = page_ptrs;

    if (line_number < 9)
    {
	pc_pos = line_number;
	for (int l = 0; l < line_number; l++)
	    marker = marker->next_ptr;
	page_top = marker;
	skip_lines = 0;

	redraw_area();
	redraw_pc();
    }
    else
    {
	pc_pos = 10;
	skip_lines = line_number -10;
	for (int k = 0; k < skip_lines; k++)
	    marker = marker->next_ptr;
	page_top = marker;

	redraw_area();
	redraw_pc();
    }
}

void ScrollingTextPC::event(Event &e)
{
    if (e.type() == Event::down)
    {
	if ((e.pointer_button() == Event::left) || (e.pointer_button() == Event::right))
	{
	    if (e.pointer_button() == Event::right)
	    {
		pc_pos--;
		if (pc_pos < 0)
		    pc_pos = 0;
		if (pc_pos < 10)
		{
		    skip_lines--;

		    if (skip_lines < 0)
			skip_lines = 0;
		    else
		    {
			pc_pos++;
			page_top = page_top->last_ptr;
		    }

		    redraw_area();
		}
		redraw_pc();
	    }
	    else
	    {
		pc_pos++;
		if ((pc_pos > 19) && (skip_lines < lines -19))
		{
		    skip_lines++;
		    pc_pos--;
		    page_top = page_top->next_ptr;
		    redraw_area();
		}
		
		if (skip_lines >= lines -20)
		    pc_pos--;
		else
		    redraw_pc();
	    }
	}
    }
}


