#include <InterViews/patch.h>
#include <InterViews/listener.h>
#include <InterViews/handler.h>
#include <InterViews/glyph.h>
#include <stdio.h>

class ScrollingText : public Listener, public Handler
{
    Patch* me;
    char *text;
    int lines, c_count, skip_lines;
    int page_top, nbox;
    const Color *fg;
    const Font *f;
    
    char *next_string();
    void alter_text();
    void redraw_area();

public:
    ScrollingText(char*, Patch*, World*);
    virtual ~ScrollingText();

    void update_text(int, char*);
    virtual void event(Event&);
};

ScrollingText::ScrollingText(char *wds, Patch *body, World *w) : Listener(body, this)
{
    button(true, Event::any);

    me = body;
    c_count = 0;
    text = wds;
    nbox = 3;
    lines = 10;

    f = w->font();
    fg = w->foreground();

    page_top = 0;
    skip_lines = 0;

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
    char *ptr = text;
    int k = 0;

    while ((k < skip_lines) && (k < c_count))
    {
	ptr += strlen(ptr) +1;
	k++;
    }

    if (k < c_count)
    {
	skip_lines++;
	return ptr;
    }
    else
	return (char*) 0;
}

void ScrollingText::redraw_area()
{
    char *c;

    for (int p = 0; p < nbox; p++)
    {
	me->remove(p);
	TBBox *nubox = new TBBox(lines);
	for (int l = 0; l < lines; l++)
	{
	    c = next_string();
	    if (c == (char*) 0)
		c = " ";
	    Patch *msg = new Patch(new VCenter(Message(f, fg, c, 0), 1.0));
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
	    skip_lines = page_top;
	    if (e.pointer_button() == Event::right)
	    {
		skip_lines -= 2;
		page_top = skip_lines +1;
		if (skip_lines < 0)
		{
		    skip_lines = 0;
		    page_top = 1;
		}
	    }
	    else
	    {
		page_top++;
		if (page_top > c_count+1)
		    page_top = 0;
	    }

	    redraw_area();
	}
    }
}

