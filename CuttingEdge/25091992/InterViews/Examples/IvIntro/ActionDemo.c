#include <InterViews/world.h>
#include <InterViews/window.h>
#include <InterViews/background.h>
#include <InterViews/margin.h>
#include <InterViews/label.h>

#include "responder.h"

main(int argc, char *argv[])
{
    World world("ActionDemo", argc, argv);

    ApplicationWindow window(new Responder(new Background(new Margin(
			 new Label("Press me",
				   world.font(), 
				   world.foreground()), 50.0), 
			 world.background()),
                             new Upper()));

    window.map();

    world.run();
}
