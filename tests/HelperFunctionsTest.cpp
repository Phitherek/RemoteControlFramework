#include <librcfcommon/librcfcommon.h>
#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
    RCF::Common::HelperFunctions hf;
    cout << "getPlainHomeDirectory():" << endl;
    cout << "Function result: " << hf.getPlainHomeDirectory() << endl;
    cout << "getHomeDirectory():" << endl;
    cout << "Function result: " << hf.getHomeDirectory().string() << endl;
    return EXIT_SUCCESS;
}
