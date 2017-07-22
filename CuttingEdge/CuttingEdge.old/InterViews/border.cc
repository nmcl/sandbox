#include <InterViews/world.h>
#include <InterViews/window.h>
#include <InterViews/background.h>
#include <InterViews/margin.h>
#include <InterViews/box.h>
#include <InterViews/center.h>
#include <InterViews/border.h>
#include <InterViews/fixedspan.h>
#include <InterViews/glue.h>
#include <InterViews/label.h>

#define RECTANGLE(w,h,a) \
(new HCenter(new Border(new FixedSpan(new FixedSpan(nil, Dimension_Y, h), \
				      Dimension_X, w), \
			world.foreground()), a))

main (int argc, char *argv[])
{
    World world("AlignDemo", argc, argv);

    TBBox box(RECTANGLE(200.0, 25.0, 0.9),
	      RECTANGLE(200.0, 25.0, 0.1),
	      RECTANGLE(200.0, 25.0, 0.2),
	      RECTANGLE(200.0, 25.0, 0.8),
	      RECTANGLE(200.0, 25.0, 0.3),
	      RECTANGLE(200.0, 25.0, 0.7),
	      RECTANGLE(200.0, 25.0, 0.5),
	      RECTANGLE(200.0, 25.0, 0.6),
	      RECTANGLE(200.0, 25.0, 0.4));
		  
    ApplicationWindow wight(new Background(new Margin(&box, 30.0),
					    world.background()));
    Font* f = world.font();
    Color* fg = world.foreground();
    Window* linton = new TopLevelWindow(
	new Background(new Margin(
	    new LRBox(
		new VCenter(
		    new Label("color", f, fg),
		    0.5
		),
		new HGlue(3.0, 0, 0),
		new VCenter(
		    new TBBox(
			new Label("red", f, fg),
			new Label("green", f, fg),
			new Label("blue", f, fg)
		    ),
		    0.5
		)
	    ), 50.0),
	    world.background()
	)
    );

    wight.map();
    linton->map();

    world.run();
}




