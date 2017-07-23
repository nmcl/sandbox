#include <InterViews/listener.h>
#include <InterViews/handler.h>
#include <InterViews/glyph.h>
#include <InterViews/sensor.h>
#include <InterViews/window.h>
#include <InterViews/patch.h>
#include <InterViews/world.h>

/* This class provides a selectable glyph. By clicking on it it will become active.
 * Because it is for use with register elements it must have a value associated with it.
 */

class Selector : public Listener, public Handler
{
    int i;            // The value of the register being displayed
    char *name;       // The name of the register being displayed
    Patch *me;        // The patch which displays the information
    const Font *ft;   // Font to display in
    const Color *cl;  // Color to display in
public:
    Selector(Patch *body, int r, char* nm, const Font*, const Color*);
    virtual ~Selector();

    virtual void event(Event&);
    void increment_value(int);  // Increment value by amount
    void set_value(int);        // Set value
    void display();             // Display name and value
};
