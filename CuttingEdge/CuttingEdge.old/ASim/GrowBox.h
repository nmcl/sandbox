#include <InterViews/box.h>
#include <InterViews/font.h>
#include <InterViews/glyph.h>
#include <InterViews/label.h>
#include <InterViews/color.h>

class LineServer {
public:
    LineServer (char *txt=0);
    virtual ~LineServer();
    virtual const char *GetLine(long lineno) const;	// lines numbered from zero
};

class GrowBox : public TBBox
{
public:
    GrowBox(int MaxLines, LineServer *l, int InitialFocus=0);
    virtual ~GrowBox();
    virtual void request(Requisition&) const;
    virtual void allocate(Canvas*,const Allocation&, Extension&);

    void UpdateText(int, const char*);	// Used to update portions of the text
    void SetFocus(int);	        // Used to change focus to a specific line

protected:
    long total_lines;	  // The total number of lines of the 'source' string
    long pc_pos;	  // The current 'focus' line
    long top, bottom;	  // The current top and bottom lines displayed
    Label **lines;	  // ptrs to glyphs for individual lines
    LineServer *srv;	  // Where to get text
    const Font *MyFont;	  // font for text
    const Color *MyColor; // color for text

    void UpdateDraw();	  // to update display after changes
    Label *GetLine(long lineno);	// lines numbered from zero
};
