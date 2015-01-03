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
        RCF::Server::User* u1 = RCF::Server::User::load("Test"); // After running ServerUserTest this already exists
        RCF::Server::User* u2 = new RCF::Server::User("Test2", "test2");
        RCF::Server::User* u3 = new RCF::Server::User; // Invalid user
        RCF::Server::User* u4 = new RCF::Server::User("Test4", "test4"); // Not in group
        try {
            u2->save();
            u4->save();
        } catch(RCF::Common::FilesystemException& e) {
            cout << "Already exists..." << endl;
        }
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
               RCF::Server::User* ru = g.getNextUser();
               if(ru->getName() == "Test") {
                   t1 = true;
               } else if(ru->getName() == "Test2") {
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
       try {
           g.save();
       } catch(RCF::Common::FilesystemException& e) {
           cout << "Already exists..." << endl;
       }
       RCF::Server::Group* g2 = RCF::Server::Group::load("testgroup");
       t1 = false, t2 = false;
       while(!g2->usersAtEnd()) {
          try {
               RCF::Server::User* ru = g2->getNextUser();
               if(ru->getName() == "Test") {
                   t1 = true;
               } else if(ru->getName() == "Test2") {
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
       g2->resetUsersIterator();
       RCF::Server::Group g3 = g;
       t1 = false, t2 = false;
       while(!g3.usersAtEnd()) {
           try {
               RCF::Server::User* ru = g3.getNextUser();
               if(ru->getName() == "Test") {
                   t1 = true;
               } else if(ru->getName() == "Test2") {
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
               RCF::Server::User* ru = g3.getNextUser();
               if(ru->getName() == "Test") {
                   t1 = true;
               } else if(ru->getName() == "Test2") {
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
       delete u2;
       delete u3;
       delete u4;
    } catch(exception& e) {
        cout << "Caught exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    cout << "Success!" << endl;
    return EXIT_SUCCESS;
}
