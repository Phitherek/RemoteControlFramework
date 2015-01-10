#include "App.h"
using namespace wxRCF;

wxIMPLEMENT_APP(App);

bool App::OnInit() {
    mainWindow* mw = new mainWindow(NULL);
    mw->Show();
    return true;
}
