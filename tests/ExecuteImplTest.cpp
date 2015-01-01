#include "librcfserver/librcfserver.h"
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

int main() {
    std::string out, err;
    out = "";
    err = "";
    int code;
    code = RCF::Server::ExecuteImpl::execute("./testcmd", &out, &err);
    cout << "Code: " << code << endl << "Stdout: " << endl << out << endl << "Stderr: " << endl << err << endl;
    return EXIT_SUCCESS;
}
