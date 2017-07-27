#include <InterViews/world.h>
#include <InterViews/window.h>
#include <InterViews/page.h>

#include "basic.h"

main (int argc, char *argv[])
{
    World world("PageDemo", argc, argv);

    Page page(new Basic(400.0, 400.0));
    page.append(new Basic(100.0, 75.0));
    page.move(0, 20.0, 50.0);

    ApplicationWindow window(&page);

    window.map();

    world.run();
}
