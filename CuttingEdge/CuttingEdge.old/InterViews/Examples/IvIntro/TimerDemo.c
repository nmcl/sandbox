#include <InterViews/world.h>
#include <InterViews/window.h>
#include <InterViews/background.h>
#include <InterViews/margin.h>
#include <InterViews/label.h>

#include "timer.h"

main(int argc, char *argv[])
{
    World world("TimerDemo", argc, argv);

    ApplicationWindow window(new Tocker(new Background(new Margin(
			 new Label("Click to Tick/Tock",
				   world.font(),
				   world.foreground()), 50.0),
                         world.background()),
			     new Ticker()));


    window.map();

    world.run();
}
