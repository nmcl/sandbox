#include <InterViews/page.h>

#include "list.h"

#include "normalpage.h"

NormalPageInfo::NormalPageInfo() : x_(0.0), y_(0.0) {}

declareList(NormalPageInfo_List, NormalPageInfo);
implementList(NormalPageInfo_List, NormalPageInfo);

NormalPage::NormalPage(Glyph *g) 
          : Page(g), info_(new NormalPageInfo_List())
{
}

void NormalPage::allocate(Canvas *c, Allocation &a, Extension &ext)
{
    Coord width = a.allotment(Dimension_X).span();
    Coord height = a.allotment(Dimension_Y).span();

    for (int index = 0;  index < count();  index++)
    {
        NormalPageInfo &info = info_->item(index);
        Page::move(index, info.x_ * width, info.y_ * height);
    }

    Page::allocate(c, a, ext);
}

void NormalPage::append(Glyph *g)
{
    NormalPageInfo info;

    info_->append(info);

    Page::append(g);
}

void NormalPage::prepend(Glyph *g)
{
    NormalPageInfo info;

    info_->prepend(info);
     Page::prepend(g);
}

void NormalPage::insert(int index, Glyph *g)
{
    NormalPageInfo info;

    info_->insert(index, info);

    Page::insert(index, g);
}

void NormalPage::remove(int index)
{
    info_->remove(index);

    Page::remove(index);
}

void NormalPage::location(int index, float &x, float &y)
{
    NormalPageInfo &info = info_->item(index);

    x = info.x_;
    y = info.y_;
}

void NormalPage::move(int index, float x, float y)
{
    NormalPageInfo &info = info_->item(index);

    info.x_ = x;
    info.y_ = y;
}
