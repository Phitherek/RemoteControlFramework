#include <librcfserver/librcfserver.h>
#include <librcfcommon/librcfcommon.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

int main() {
    RCF::Server::Command c1("test1", "echo Test successful!");
    RCF::Server::Command c2("test2", "echo Works {rcfparam:0} and {rcfparam:1}!", 2);
    if(c1.getName() != "test1" || c2.getName() != "test2") {
        cerr << "getName() failed!" << endl;
        return EXIT_FAILURE;
    }
    if(c1.getExec() != "echo Test successful!") {
        cerr << "getExec() 1 failed!" << endl;
        return EXIT_FAILURE;
    }
    try {
        c2.getExec();
    } catch(RCF::Common::ParametersNeededException& e) {
        cout << "Cannot execute parametrized command without parameters - OK!" << endl;
    }
    vector<string> params;
    params.push_back("good");
    if(c1.getExec(params) != "echo Test successful!") {
        cerr << "getExec() 2 failed (with noparam command)" << endl;
        return EXIT_FAILURE;
    }
    try {
        c2.getExec(params);
    } catch(RCF::Common::ParametersNeededException& e) {
        cout << "Cannot execute parametrized command with too few parameters - OK!" << endl;
    }
    params.push_back("better");
    if(c2.getExec(params) != "echo Works good and better!") {
        cerr << "getExec() 2 failed (with parametrized command and sufficient nr of params)" << endl;
        return EXIT_FAILURE;
    }
    params.push_back("best");
    if(c2.getExec(params) != "echo Works good and better!") {
        cerr << "getExec() 2 failed (with parametrized command and too many params)" << endl;
        return EXIT_FAILURE;
    }
    cout << "Success!" << endl;
    return EXIT_SUCCESS;
}
