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
        code = RCF::Server::ExecuteImpl::execute("testcmd", &out, &err);
    #endif
    cout << "Code: " << code << endl << "Stdout: " << endl << out << endl << "Stderr: " << endl << err << endl;
    return EXIT_SUCCESS;
}
