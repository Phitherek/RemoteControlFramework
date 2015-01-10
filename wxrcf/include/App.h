#ifndef _APP_H
#define _APP_H
#include <boost/predef.h>
#if BOOST_OS_WINDOWS
    #define _WIN32_WINNT 0x0501
    #define WINVER 0x0501
    #define WIN32_LEAN_AND_MEAN
    #define WX_LEAN_AND_MEAN
#endif
#include <boost/asio.hpp>
#include "layout.h"
#include <wx/wx.h>

namespace wxRCF {
    class App: public wxApp {
    public:
        virtual bool OnInit();
    };
}

#endif
