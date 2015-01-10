#ifndef _APP_H
#define _APP_H
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "layout.h"

namespace wxRCF {
    class App: public wxApp {
    public:
        virtual bool OnInit();
    };
}

#endif
