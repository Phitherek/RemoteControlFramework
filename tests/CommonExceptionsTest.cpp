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
    try {
        throw RCF::Common::NotFoundException("sth", "Object not found!");
    } catch(RCF::Common::NotFoundException& e) {
        if(strcmp(e.what(), "sth: Object not found!") != 0) {
            fail = true;
        }
    }
    if(fail) {
        cout << "NotFoundException is wrong!" << endl;
        return EXIT_FAILURE;
    }
    try {
        throw RCF::Common::AlreadyExistsException("sth", "Object already exists!");
    } catch(RCF::Common::AlreadyExistsException& e) {
        if(strcmp(e.what(), "sth: Object already exists!") != 0) {
            fail = true;
        }
    }
    if(fail) {
        cout << "AlreadyExistsException is wrong!" << endl;
        return EXIT_FAILURE;
    }
    try {
        throw RCF::Common::AtEndException("Collection at end");
    } catch(RCF::Common::AtEndException& e) {
        if(strcmp(e.what(), "Collection at end") != 0) {
            fail = true;
        }
    }
    if(fail) {
        cout << "AtEndException is wrong!" << endl;
        return EXIT_FAILURE;
    }
    try {
        throw RCF::Common::ParametersNeededException("Insufficient parameters for test command!", 3, 1);
    } catch(RCF::Common::ParametersNeededException& e) {
        if(strcmp(e.what(), "Insufficient parameters for test command! (3 parameters required, 1 given)") != 0) {
            fail = true;
        }
    }
    if(fail) {
        cout << "ParametersNeededException is wrong!" << endl;
        return EXIT_FAILURE;
    }
    try {
        throw RCF::Common::RuntimeException("Something went wrong");
    } catch(RCF::Common::RuntimeException& e) {
        if(strcmp(e.what(), "Something went wrong") != 0) {
            fail = true;
        }
    }
    if(fail) {
        cout << "RuntimeException is wrong!"  << endl;
        return EXIT_FAILURE;
    }
    cout << "Success!" << endl;
    return EXIT_SUCCESS;
}
