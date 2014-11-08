#include <librcfserver/Permission.h>
#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
    RCF::Server::Permission perm("test");
    if(perm.getName() != "test") {
        cerr << "getName() failed" << endl;
        return EXIT_FAILURE;
    }
    if(perm.getPermissionType() != "permission") {
        cerr << "getPermissionType() failed" << endl;
        return EXIT_FAILURE;
    }
    cout << "Success!" << endl;
    return EXIT_SUCCESS;
}
