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

public:
    ScrollingText(char*, Patch*, World*, int, int);
    virtual ~ScrollingText();

    void update_text(int, char*);
    virtual void event(Event&);
};

ScrollingText::ScrollingText(char *wds, Patch *body, World *w, int size, int boxes) : Listener(body, this)
{
    button(true, Event::any);

    me = body;
    c_count = 0;
    text = wds;
    nbox = boxes;

    f = w->font();
    fg = w->foreground();
    lines = size;
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

    while (k < skip_lines)
    {
	ptr += strlen(ptr) +1;
	k++;
	if (k > c_count)
	{
	    ptr = text;
	    skip_lines = 0;
	}
    }

    skip_lines++;

    return ptr;
}

void ScrollingText::update_text(int jump_char, char* new_text)
{
    char *ptr = text;

    for (int k = 0; k < jump_char; k++)
	ptr++;

    bcopy(new_text, ptr+1, strlen(new_text));
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

//	    me->remove(1);
//	    TBBox *b = new TBBox(nbox);
	    for (int p = 0; p < nbox; p++)
	    {
		me->remove(p);
		TBBox *nubox = new TBBox(lines);
		for (int l = 0; l < lines; l++)
		{
		    Patch *msg = new Patch(new VCenter(Message(f, fg, next_string(), 0), 1.0));
		    nubox->insert(l, msg);
		}
		me->insert(p, nubox);
	    }
//	    me->insert(1, b);
	    me->reallocate();
	    me->redraw();
	}
    }
}
