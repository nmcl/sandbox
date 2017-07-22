#include <InterViews/background.h>
#include <InterViews/session.h>
#include <InterViews/style.h>
#include <InterViews/window.h>
#include <InterViews/color.h>
#include <InterViews/box.h>
#include <InterViews/background.h>

#include "ActiveColour.h"
#include "ActiveColourEdit.h"

int main(int argc, char** argv)
{
    Session* session = new Session("- Driver -", argc, argv);
    Style*   style   = session->style();

    int res;

    char uidName[48];

    cout << "Example uid : 1358972318.689335990.577536\n";
    cout << "Uid : ";

    cin >> uidName;

    Uid u(uidName);

    res = 0;

    ActiveColour*     ac  = ActiveColour::Create(res, u);

    if (res == 1)
    {
	ActiveColourEdit* ace = new ActiveColourEdit(ac);

	Background* bg = new Background(ace, style->background());

	ApplicationWindow* win = new ApplicationWindow(bg);

	session->run_window(win);
    }
    else
	cerr << "Unable to create ""ActiveColour""\n";

    ActiveColour::Destroy();
}

