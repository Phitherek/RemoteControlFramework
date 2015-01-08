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
    cout << "Enter something: ";
    std::string sensitive;
    hf.setInputEcho(false);
    cin >> sensitive;
    hf.setInputEcho(true);
    cout << endl;
    cout << "You entered: " << sensitive << endl;
    return EXIT_SUCCESS;
}
