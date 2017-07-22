#include <InterViews/world.h>
#include <InterViews/window.h>

#include "normalpage.h"
#include "basic.h"

main (int argc, char *argv[])
{
    World world("NormalDemo", argc, argv);

    NormalPage page(new Basic(400.0, 400.0));
    page.append(new Basic(100.0, 75.0));
    page.move(0, 0.05, 0.125);

    ApplicationWindow window(&page);

    window.map();

    world.run();
}
