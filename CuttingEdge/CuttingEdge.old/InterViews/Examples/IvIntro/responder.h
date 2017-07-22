#include <InterViews/listener.h>
#include <InterViews/handler.h>
#include <InterViews/action.h>

class Upper
{
public:
    void Up();
};

declare(ActionCallback, Upper);

class Responder : public Listener, public Handler
{
public:
    Responder(Glyph*, Upper*);

    void event(Event&);
private:
    ActionCallback(Upper) *up;
    Upper *upper;
};
