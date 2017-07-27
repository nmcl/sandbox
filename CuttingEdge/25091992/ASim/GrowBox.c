#include <iostream.h>
#include <InterViews/label.h>
#include <InterViews/world.h>

#include "GrowBox.h"

LineServer::LineServer (char *) { }
LineServer::~LineServer() { }
const char *LineServer::GetLine(long ) const { return ""; }

static const int InitialSize = 15;
Label *GrowBox::GetLine(long lno)
{
    // Sanity check on input args
    if ((lno<0) || (lno>=total_lines)) {
	cerr << "Attempt to access line " << lno+1 
	     << " of a " << total_lines << "-line box?" << endl;
	lno = 0;
    }

    // Ensure that necessary lines are fetched and glyphs created
    if (lines[lno] == 0)
	lines[lno] = new Label(srv->GetLine(lno), MyFont, MyColor);

    // easy as that!
    return lines[lno];
}

GrowBox::GrowBox(int MaxLines, LineServer *l, int InitialFocus) : TBBox(InitialSize)
{
    total_lines = MaxLines;
    pc_pos = InitialFocus;
    top = bottom = 0;
    lines = new Label *[MaxLines];
    for (long lno=0; lno<total_lines; lno++) lines[lno] = 0;
    srv = l;
    MyFont = World::current()->font();
    MyColor = World::current()->foreground();

    // Pre-load the box with the first N entries so its knows how big it is
    for (lno=0; lno<InitialSize; lno++) {
	lines[lno] = new Label(srv->GetLine(lno), MyFont, MyColor);
	append(lines[lno]);
    }

    UpdateDraw();
}

GrowBox::~GrowBox()
{
    for (long lno=0; lno<total_lines; lno++)
	delete lines[lno];
    delete lines;
}

void GrowBox::request(Requisition &r) const
{
    Box::request(r);
    Requirement& heightreq = r.requirement(Dimension_Y);
    Coord shrinkability = heightreq.natural() - MyFont->size();
    Coord stretchability = total_lines * MyFont->size() - heightreq.natural();

    cerr << "request: "
	 << "natural " << heightreq.natural() 
	 << ", font size " << MyFont->size()
	 << ", total lines " << total_lines
	 << ", shrink " << shrinkability
	 << ", stretch " << stretchability
	 << endl;
    
    heightreq.shrink(shrinkability);
    heightreq.stretch(stretchability);
}

void GrowBox::allocate(Canvas*c,const Allocation&a, Extension&e)
{
    // Compute requested height in terms of lines
    const Allotment &height = a.allotment(Dimension_Y);
    int requestedlines = height.span()/MyFont->size();

    // Adjust for reality
    if (requestedlines<=0) requestedlines = 1;
    if (requestedlines>total_lines) requestedlines = total_lines;

    cerr << "allocate: "
	 << "span " << height.span()
	 << ", font size " << MyFont->size()
	 << ", requested lines " << requestedlines
	 << ", count " << count()
	 << ", bottom " << bottom
	 << endl;

    // Grow or shrink the box as necessary
    if (requestedlines>count()) {	 /* need to grow */
	for (long i=count()+1; i<=requestedlines; i++) {
	    long lno=++bottom;
	    append(GetLine(lno));
	}
    } else if (requestedlines<count()) { /* need to shrink */
	for (long i=count()-1; i>=requestedlines; --i) {
	    remove(i);
	    bottom -= 1;
	}
    }

    // Get the Box to do the proper allocation now
    Box::allocate(c,a,e);
}

void GrowBox::UpdateText(int idx, const char*s)
{
    lines[idx] = new Label(s, MyFont, MyColor);
    if ((idx >= top) && (idx <= bottom))
	UpdateDraw();
}

void GrowBox::SetFocus(int idx)
{
    pc_pos = idx;
    UpdateDraw();
}


void GrowBox::UpdateDraw()
{
    // Recompute top and bottom based on focus, and current window size
    if ((pc_pos <= top) || (pc_pos >=bottom)) {
	long sizeofbox = count();	// get current size of box
	top = pc_pos < (sizeofbox/2) ? 0 : pc_pos - (sizeofbox/2);
	bottom = sizeofbox + top - 1;
    }
    
    // update the tbbox entries
    for (long dline=0; dline<bottom-top; dline++)
	replace (dline, GetLine(dline+top));
}
