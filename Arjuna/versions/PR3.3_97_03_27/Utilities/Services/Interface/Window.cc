/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $id:$
 */



#include <InterViews/scrbox.h>
#include <IV-look/kit.h>
#include <strstream.h>

#include "Window.h"
#include "Message.h"


extern TBScrollBox* scrBox;
extern Glyph* bar;
extern Patch* patch;


ScrollingText::ScrollingText (char *wds, FieldEditor* fe)
{
    text = wds;
    fld = fe;
    convertText();
}

ScrollingText::~ScrollingText ()
{
    if (text)
	::delete [] text;
}

void ScrollingText::convertText ()
{
    if (text == (char*) 0)
	return;
    
    istrstream text_source(text);

    do
    {
	char* line = ::new char[1025];
    
	::memset(line, '\0', 1025);
	text_source.getline(line, 1024);
	
	if (text_source.eof() == 0)
	{
	    Selector* slc = new Selector(fld, line);
	    scrBox->append(slc);
	}
    } while (text_source.eof() == 0);
}

void ScrollingText::change_text (char *new_text)
{
    GlyphIndex size = scrBox->count();

    for (int i = 0; i < size; i++)
	scrBox->remove(0);

    if (text != (char*) 0)
	::delete [] text;

    if (new_text == (char*) 0)
	return;
    
    text = new_text;
    convertText();

    scrBox->begin_adjustment(Dimension_Y);

    while (!scrBox->shown(0))
	scrBox->scroll_backward(1);

    scrBox->commit_adjustment(Dimension_Y);

    bar->change(0);

    patch->reallocate();
    patch->redraw();
}
