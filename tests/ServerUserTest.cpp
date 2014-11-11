#include <librcfserver/librcfserver.h>
#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
    RCF::Server::User u("Test", "testtest");
    if(u.getPermissionType() != "user") {
        cout << "GetPermissionType() failed!" << endl;
        return EXIT_FAILURE;
    }
    if(!u.checkPassword("testtest")) {
        cout << "CheckPassword() failed!" << endl;
        return EXIT_FAILURE;
    }
    if(u.checkPassword("ureuhe")) {
        cout << "CheckPassword() failed!" << endl;
        return EXIT_FAILURE;
    }
    u.save();
    cout << "User data has been saved!" << endl;
    RCF::Server::User u2;
    u2.load("Test");
    if(u2.getPermissionType() != "user") {
        cout << "load() failed!" << endl;
        return EXIT_FAILURE;
    }
    if(!u2.checkPassword("testtest")) {
        cout << "load() failed!" << endl;
        return EXIT_FAILURE;
    }
    if(u2.checkPassword("ureuhe")) {
        cout << "load() failed!" << endl;
        return EXIT_FAILURE;
    }
    cout << "Success!" << endl;
    return EXIT_SUCCESS;
}
