#include "Scroll.h"
#include "Message.h"

ScrollingText::ScrollingText(char *wds, Patch *body, World *w) : Listener(body, this)
{
    button(true, Event::any);

    me = body;
    c_count = 0;
    skip_lines = 0;
    text = wds;
    page_top = text;
    marker = text;

    f = w->font();
    fg = w->foreground();

    alter_text();
}

ScrollingText::~ScrollingText()
{
}

void ScrollingText::alter_text()
{
    char *ptr;

    ptr = text;
    while (*ptr != '\0')
    {
	if (*ptr == '\n')
	{
	    c_count++;
	    *ptr = '\0';
	}
	if ((*ptr == '\0') && (*(ptr+1) == '\0'))
	{
	    c_count--;
	    *(ptr+1) = ' ';
	}
	ptr++;
    }
}

char *ScrollingText::next_string()
{
    char *ptr;

    if (skip_lines < c_count)
    {
	ptr = marker;
	marker += strlen(marker) +1;

	return ptr;
    }
    else
    {
	skip_lines = c_count;
	marker = page_top;
	ptr = " ";

	return ptr;
    }
}

void ScrollingText::redraw_area()
{
    for (int p = 0; p < 3; p++)
    {
	me->remove(p);
	TBBox *nubox = new TBBox(10);
	for (int l = 0; l < 10; l++)
	{
	    Patch *msg = new Patch(new VCenter(Message(f, fg, next_string()), 1.0));
	    nubox->insert(l, msg);
	}
	me->insert(p, nubox);
    }
    me->reallocate();
    me->redraw();
}

void ScrollingText::update_text(int jump_char, char* new_text)
{
    char *ptr = text;

    for (int k = 0; k < jump_char; k++)
	ptr++;

    bcopy(new_text, ptr, strlen(new_text));
    redraw_area();
}

void ScrollingText::event(Event &e)
{
    if (e.type() == Event::down)
    {
	if ((e.pointer_button() == Event::left) || (e.pointer_button() == Event::right))
	{
	    if (e.pointer_button() == Event::right)
	    {
		skip_lines--;
		marker = text;
		if (skip_lines < 0)
		{
		    skip_lines = 0;
		    marker = text;
		}
		else
		{
		    for (int k = 0; k < skip_lines; k++)
			marker += strlen(marker) +1;
		}
	    }
	    else
	    {
		marker = page_top;
		skip_lines++;
		marker += strlen(marker) +1;
	    }

	    page_top = marker;
	    redraw_area();
	}
    }
}

