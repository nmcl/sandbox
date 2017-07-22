#include <locale.h>

#include <iostream.h>

#include <InterViews/session.h>
#include <InterViews/patch.h>
#include <InterViews/window.h>
#include <InterViews/background.h>

#include <IV-look/kit.h>
#include <InterViews/layout.h>

#include "Main.h"
#include "BasicActionInteractor.h"
#include "SerialActions.h"
#include "ConcurrentActions.h"

Patch *patch = NULL;

int main(int argc, char *argv[])
{
    int res;

    Session* session = new Session("ActionStructureEdit", argc, argv, NULL, NULL);
    
    ActionStructure* asC = new ConcurrentActions;
    ActionStructure* asS = new SerialActions;

    res = asC->AddBelow(new BasicActionInteractor);
    cout << "Main() res = " << res << '\n';
    res = asC->AddBelow(new BasicActionInteractor);
    cout << "Main() res = " << res << '\n';
    res = asS->AddAfter(new BasicActionInteractor);
    cout << "Main() res = " << res << '\n';
    res = asS->AddAfter(new BasicActionInteractor);
    cout << "Main() res = " << res << '\n';
    res = asC->AddBelow(asS);
    cout << "Main() res = " << res << '\n';

    Window *window = NULL;
    
    patch = new Patch(asC);

    window = new ApplicationWindow(
		 new Background(
		     patch,
		     WidgetKit::instance()->background()
		 )
	     );

    return session->run_window(window);
}
