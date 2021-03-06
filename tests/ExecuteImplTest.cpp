#include "librcfserver/librcfserver.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <boost/predef.h>
using namespace std;

int main() {
    std::string out, err;
    out = "";
    err = "";
    int code;
    #if BOOST_OS_LINUX || BOOST_OS_MACOS
        code = RCF::Server::ExecuteImpl::execute("./testcmd", &out, &err);
    #elif BOOST_OS_WINDOWS
        code = RCF::Server::ExecuteImpl::execute("testcmd.exe", &out, &err);
    #endif
    cout << "Testcmd" << endl << "Code: " << code << endl << "Stdout: " << endl << out << endl << "Stderr: " << endl << err << endl;
    out = "";
    err = "";
    #if BOOST_OS_LINUX || BOOST_OS_MACOS
        code = RCF::Server::ExecuteImpl::execute("./testcmdd", &out, &err);
    #elif BOOST_OS_WINDOWS
        code = RCF::Server::ExecuteImpl::execute("testcmdd.exe", &out, &err);
    #endif
    cout << "Testcmdd" << endl << "Code: " << code << endl << "Stdout: " << endl << out << endl << "Stderr: " << endl << err << endl;
    out = "";
    err = "";
    #if BOOST_OS_LINUX || BOOST_OS_MACOS
        code = RCF::Server::ExecuteImpl::execute("./testcmd2 good better", &out, &err);
    #elif BOOST_OS_WINDOWS
        code = RCF::Server::ExecuteImpl::execute("testcmd2.exe good better", &out, &err);
    #endif
    cout << "Testcmd2" << endl << "Code: " << code << endl << "Stdout: " << endl << out << endl << "Stderr: " << endl << err << endl;
    return EXIT_SUCCESS;
}
