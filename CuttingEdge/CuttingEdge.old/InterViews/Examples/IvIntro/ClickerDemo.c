#include <InterViews/world.h>
#include <InterViews/window.h>
#include <InterViews/background.h>
#include <InterViews/margin.h>
#include <InterViews/label.h>

#include "leftclicker.h"

main(int argc, char *argv[])
{
    World world("ClickerDemo", argc, argv);

    ApplicationWindow window(new LeftClicker(new Background(new Margin(
		         new Label("Click me", 
			            world.font(), 
			            world.foreground()), 50.0),
			 world.background())));

    window.map();

    world.run();
}

