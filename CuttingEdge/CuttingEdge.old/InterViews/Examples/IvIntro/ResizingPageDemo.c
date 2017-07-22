#include <InterViews/world.h>
#include <InterViews/window.h>

#include "resizingpage.h"
#include "basic.h"
#include "adaptable.h"
#include "elastic.h"

main (int argc, char *argv[])
{
    const Coord initialWidth = 400.0, initialHeight = 400.0;

    World world("ResizingDemo", argc, argv);

    ResizingPage page(new Basic(initialWidth, initialHeight));

    int index = 0;
    page.appendResizable(new Adaptable(100.0, 75.0));
    page.move(index++, 0.05, 0.125);
    page.append(new Basic(100.0, 75.0, fil, fil, 1.0, fil, fil, 1.0));
    page.move(index++, 0.95, 0.875);
    page.appendResizable(new HElastic());
    page.move(index++, 0.0, 0.5);
    page.appendResizable(new VElastic());
    page.move(index++, 0.5, 0.0);

    ApplicationWindow window(&page);

    window.map();

    world.run();
}
