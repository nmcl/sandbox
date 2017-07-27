#include "list.h"
#include "resizable.h"
#include "resizingpage.h"

ResizingPageInfo::ResizingPageInfo() : isResizable(false) {}

declareList(ResizingPageInfo_List, ResizingPageInfo);
implementList(ResizingPageInfo_List, ResizingPageInfo);

ResizingPage::ResizingPage(Glyph *g)
            : NormalPage(g), resizables(new ResizingPageInfo_List()) {}

inline boolean ResizingPage::IsResizable(int index)
{
    return resizables->item(index).isResizable;
}

void ResizingPage::allocate(Canvas *c, Allocation &a, Extension &ext)
{
    Coord width = a.allotment(Dimension_X).span();
    Coord height = a.allotment(Dimension_Y).span();

    for (int index = 0;  index < count();  index++)
    {
        if (IsResizable(index))
        {
	    ((Resizable *) component(index))->resize(width, height);
	    change(index);
        }
    }

    NormalPage::allocate(c, a, ext);
}

void ResizingPage::appendResizable(Glyph *g)
{
    ResizingPageInfo resizable;

    resizable.isResizable = true;
    resizables->append(resizable);

    NormalPage::append(g);
}

void ResizingPage::prependResizable(Glyph *g)
{
    ResizingPageInfo resizable;

    resizable.isResizable = true;
    resizables->prepend(resizable);

    NormalPage::prepend(g);
}

void ResizingPage::insertResizable(int index, Glyph *g)
{
    ResizingPageInfo resizable;

    resizable.isResizable = true;
    resizables->insert(index, resizable);

    NormalPage::insert(index, g);
}

void ResizingPage::removeResizable(int index)
{
    ResizingPageInfo resizable;

    resizables->remove(index);

    NormalPage::remove(index);
}

void ResizingPage::append(Glyph *g)
{
    ResizingPageInfo resizable;

    resizables->append(resizable);

    NormalPage::append(g);
}

void ResizingPage::prepend(Glyph *g)
{
    ResizingPageInfo resizable;

    resizables->prepend(resizable);

    NormalPage::prepend(g);
}

void ResizingPage::insert(int index, Glyph *g)
{
    ResizingPageInfo resizable;

    resizables->insert(index, resizable);

    NormalPage::insert(index, g);
}

void ResizingPage::remove(int index)
{
    resizables->remove(index);

    NormalPage::remove(index);
}
