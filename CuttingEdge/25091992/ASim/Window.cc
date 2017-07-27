#include "Window.h"
#include "Message.h"
#include <InterViews/world.h>

ScrollingText::ScrollingText(char *wds, Glyph *body) : Patch(body)
{
    lines = 0;
    text = wds;
    page_ptrs = new points;
    page_top = page_ptrs;
    f = World::current()->font();
    fg = World::current()->foreground();

    connect(this);
    alter_text();
    curvalue_ = lines;
    redraw_area();
}

ScrollingText::~ScrollingText()
{
    free(text);
}

Coord ScrollingText::lower(DimensionName) const { return 0.0; }
Coord ScrollingText::upper(DimensionName) const { return lines; }
Coord ScrollingText::length(DimensionName) const { return lines; }

Coord ScrollingText::cur_lower(DimensionName d) const
{
    return curvalue_ - cur_length(d);
}

Coord ScrollingText::cur_upper(DimensionName d) const
{
    return curvalue_ + cur_length(d);
}

Coord ScrollingText::cur_length(DimensionName d) const { return LINES; }

void ScrollingText::calc_pos(DimensionName d, Coord position)
{
    if (position != curvalue_)
    {
	Coord p = position;

        if (p <= LINES)
            p = LINES;
        else
	    if (p >= lines)
		p = lines;

        curvalue_ = p;
        notify();
    }
}
    
void ScrollingText::scroll_to(DimensionName d, Coord position)
{
    calc_pos(d, position);
    page_top = page_ptrs;
    for (float k = 0; k < lines - curvalue_; k++)
	page_top = page_top->next_ptr;
    
    redraw_area();
}

void ScrollingText::scroll_backward(DimensionName d)
{
    if (page_top->next_ptr != 0)
	page_top = page_top->next_ptr;

    add_text("This is a test\n");

    calc_pos(d, curvalue_ - 1.0);
}

void ScrollingText::scroll_forward(DimensionName d)
{
    if (page_top->last_ptr != 0)
	page_top = page_top->last_ptr;

    calc_pos(d, curvalue_ + 1.0);
}

void ScrollingText::set_value(Coord v)
{
    curvalue_ = v;
    notify();
}

Coord ScrollingText::get_value() const
{
    return curvalue_;
}

void ScrollingText::alter_text()
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
}

void ScrollingText::redraw_area()
{
    points *marker = page_top;
    for (int l = 0; l < LINES; l++)
    {
	Patch *msg = new Patch(new VCenter(Message(f, fg, marker->ptr), 1.0));
	if (marker->next_ptr != 0)
	    marker = marker->next_ptr;
	else
	    msg = new Blank_Line;
	this->replace(l, msg);
    }
    this->reallocate();
    this->redraw();
}

void ScrollingText::update(DimensionName)
{
    redraw_area();
}

void ScrollingText::update_all()
{
    redraw_area();
}

void ScrollingText::add_text(char* new_line)
{
    points *marker = page_top;
    char *new_text;

    new_text = malloc(strlen(new_line) +1);
    bcopy(new_line, new_text, strlen(new_line));
    new_text[strlen(new_line) -1] = '\0';
    while (marker->next_ptr != 0)
	marker = marker->next_ptr;

    lines++;
    marker->next_ptr = new points;
    marker->next_ptr->last_ptr = marker;
    marker = marker->next_ptr;
    marker->next_ptr = 0;
    marker->ptr = new_text;

    redraw_area();
}
