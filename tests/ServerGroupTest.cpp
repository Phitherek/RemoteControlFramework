#include <librcfcommon/librcfcommon.h>
#include <librcfserver/librcfserver.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <exception>
using namespace std;

int main() {
    try {
        RCF::Server::Group g("testgroup");
        if(g.getPermissionType() != "group") {
            cout << "getPermissionType() failed!" << endl;
            return EXIT_FAILURE;
        }
        RCF::Server::User u1;
        u1.load("Test"); // After running ServerUserTest this already exists
        RCF::Server::User u2("Test2", "test2");
        RCF::Server::User u3; // Invalid user
        RCF::Server::User u4("Test4", "test4"); // Not in group
        u2.save();
        u4.save();
        g.addUser(u1);
        g.addUser(u2);
        try {
            g.addUser(u3);
       } catch(RCF::Common::InvalidObjectException& e) {
           cout << "Invalid user cannot be added - OK!" << endl;
       }
       if(!g.hasUser("Test") || !g.hasUser("Test2") || g.hasUser("Test4")) {
           cout << "hasUser() failed!" << endl;
           return EXIT_FAILURE;
       }
       bool t1 = false, t2 = false;
       while(!g.usersAtEnd()) {
           try {
               RCF::Server::User ru = g.getNextUser();
               if(ru.getName() == "Test") {
                   t1 = true;
               } else if(ru.getName() == "Test2") {
                   t2 = true;
               }
           } catch(RCF::Common::AtEndException& e) {
               break;
           }
       }
       if(!t1 || !t2) {
           cout << "getNextUser() and usersAtEnd() combination failed!" << endl;
           return EXIT_FAILURE;
       }
       g.resetUsersIterator();
       g.save();
       RCF::Server::Group g2;
       g2.load("testgroup");
       t1 = false, t2 = false;
       while(!g2.usersAtEnd()) {
          try {
               RCF::Server::User ru = g2.getNextUser();
               if(ru.getName() == "Test") {
                   t1 = true;
               } else if(ru.getName() == "Test2") {
                   t2 = true;
               }
           } catch(RCF::Common::AtEndException& e) {
               break;
           }
       }
       if(!t1 || !t2) {
           cout << "File failure!" << endl;
           return EXIT_FAILURE;
       }
       g2.resetUsersIterator();
       RCF::Server::Group g3 = g;
       t1 = false, t2 = false;
       while(!g3.usersAtEnd()) {
           try {
               RCF::Server::User ru = g3.getNextUser();
               if(ru.getName() == "Test") {
                   t1 = true;
               } else if(ru.getName() == "Test2") {
                   t2 = true;
               }
           } catch(RCF::Common::AtEndException& e) {
               break;
           }
       }
       if(!t1 || !t2) {
           cout << "Copy failure!" << endl;
           return EXIT_FAILURE;
       }
       g3.removeUser("Test2");
       t1 = false, t2 = false;
       while(!g3.usersAtEnd()) {
           try {
               RCF::Server::User ru = g3.getNextUser();
               if(ru.getName() == "Test") {
                   t1 = true;
               } else if(ru.getName() == "Test2") {
                   t2 = true;
               }
           } catch(RCF::Common::AtEndException& e) {
               break;
           }
       }
       if(g3.hasUser("Test2") || t2) {
           cout << "removeUser() failed!" << endl;
           return EXIT_FAILURE;
       }
       g3.resetUsersIterator();
    } catch(exception& e) {
        cout << "Caught exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    cout << "Success!" << endl;
    return EXIT_SUCCESS;
}
