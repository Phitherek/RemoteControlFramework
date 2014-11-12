#include <librcfcommon/librcfcommon.h>
#include <iostream>
#include <cstdlib>
#include <boost/filesystem.hpp>
#include <cstring>
using namespace std;

int main() {
    bool fail = false;
    try {
        throw RCF::Common::FilesystemException(boost::filesystem::path("/some/path"), "Does not exist");
    } catch(RCF::Common::FilesystemException& e) {
        if(strcmp(e.what(), "/some/path: Does not exist") != 0) {
            fail = true;
        }
    }
    if(fail) {
        cout << "FilesystemException is wrong!"  << endl;
        return EXIT_FAILURE;
    }
    try {
        throw RCF::Common::InvalidObjectException("Invalid object");
    } catch(RCF::Common::InvalidObjectException& e) {
        if(strcmp(e.what(), "Invalid object") != 0) {
            fail = true;
        }
    }
    if(fail) {
        cout << "InvalidObjectException is wrong!"  << endl;
        return EXIT_FAILURE;
    }
    try {
        throw RCF::Common::ParserException(boost::filesystem::path("/some/path"), 40, "Test error");
    } catch(RCF::Common::ParserException& e) {
        if(strcmp(e.what(), "/some/path:40: Test error") != 0) {
            fail = true;
        }
    }
    if(fail) {
        cout << "ParserException is wrong!"  << endl;
        return EXIT_FAILURE;
    }
    cout << "Success!" << endl;
    return EXIT_SUCCESS;
}