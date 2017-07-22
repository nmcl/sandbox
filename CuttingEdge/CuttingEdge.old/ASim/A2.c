#include <iostream.h>

#include <InterViews/background.h>
#include <InterViews/display.h>
#include <InterViews/session.h>
#include <InterViews/style.h>
#include <InterViews/window.h>

#include <IV-look/adjustable.h>
#include <IV-look/scroller.h>

#include "GrowBox.h"

class ReadServer : public LineServer {
public:
    ReadServer() : LineServer() { }
    virtual ~ReadServer();
    virtual const char *GetLine(long lno) const;
};

ReadServer::~ReadServer() {}

const char *ReadServer::GetLine(long lno) const
{
    char *buf = new char[80];
    cout << lno << "? "; 
    cin >> buf;
    return buf;
}

class AdjScr : public GrowBox, public Adjustable {
public:
    AdjScr (int maxlines, LineServer *l) : GrowBox(maxlines, l) { }

    virtual Coord lower(DimensionName) const { return 0; }
    virtual Coord upper(DimensionName) const { return total_lines-1; }
    virtual Coord length(DimensionName) const { return total_lines; }
    virtual Coord cur_lower(DimensionName) const  { return top; }
    virtual Coord cur_upper(DimensionName) const  { return bottom; }
    virtual Coord cur_length(DimensionName) const { return bottom-top+1; }
};

int main(int argc, char **argv)
{
    // Create the InterViews world 
    Session MySession("GrowTest", argc, argv);

    ReadServer srv;

//    AdjScr adj(32, &srv);

    Style *style = MySession.default_display()->style();
//    VScroller vs(&adj, style);

//    LRBox hbox(&adj, &vs);

    GrowBox g(32,&srv);
    Background bg(&g, style->background());
    ApplicationWindow MyWindow(&bg);

    MySession.run_window(&MyWindow);

    return 0;
}

