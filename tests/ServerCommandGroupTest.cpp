#include <librcfserver/librcfserver.h>
#include <librcfcommon/librcfcommon.h>
#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
    RCF::Server::CommandGroup* cg = new RCF::Server::CommandGroup("testcg");
    RCF::Server::CommandGroup* cg2 = new RCF::Server::CommandGroup("testcg2");
    RCF::Server::CommandGroup* subcg = new RCF::Server::CommandGroup("testsubcg");
    if(cg->getName() != "testcg" || cg2->getName() != "testcg2" || subcg->getName() != "testsubcg") {
        cout << "getName() failed!" << endl;
        return EXIT_FAILURE;
    }
    RCF::Server::Command* c1 = RCF::Server::Command::load("test1");
    RCF::Server::Command* c2 = RCF::Server::Command::load("test2");
    cg->addCommand(c1);
    cg2->addGroup(subcg);
    subcg->addCommand(c2);
    if(!cg->hasCommand("test1") || cg->hasCommand("test2") || cg2->hasCommand("test1") || cg2->hasCommand("test2") || subcg->hasCommand("test1") || !subcg->hasCommand("test2")) {
        cout << "hasCommand() failed!" << endl;
        return EXIT_FAILURE;
    }
    if(!cg->recursiveHasCommand("test1") || cg->recursiveHasCommand("test2") || cg2->recursiveHasCommand("test1") || !cg2->recursiveHasCommand("test2") || subcg->recursiveHasCommand("test1") || !subcg->recursiveHasCommand("test2")) {
        cout << "recursiveHasCommand() failed!" << endl;
        return EXIT_FAILURE;
    }
    if(cg->hasGroup("testsubcg") || !cg2->hasGroup("testsubcg")) {
        cout << "hasGroup() failed!" << endl;
        return EXIT_FAILURE;
    }
    if(cg->recursiveHasGroup("testsubcg") || !cg2->recursiveHasGroup("testsubcg")) {
        cout << "recursiveHasGroup() failed!" << endl;
        return EXIT_FAILURE;
    }
    RCF::Server::User* u1 = RCF::Server::User::load("Test");
    RCF::Server::User* u2 = RCF::Server::User::load("Test2");
    cg->addPermission(u1);
    if(!c1->canExecute("Test")) {
        cout << "addPermission() 1 failed!" << endl;
        return EXIT_FAILURE;
    }
    cg2->addPermission(u2);
    if(c2->canExecute("Test2")) {
        cout << "addPermission() 2 failed!" << endl;
        return EXIT_FAILURE;
    }
    cg2->recursiveAddPermission(u2);
    if(!c2->canExecute("Test2")) {
        cout << "recursiveAddPermission() failed!" << endl;
        return EXIT_FAILURE;
    }
    bool success = false;
    while(!cg->commandsAtEnd()) {
        try {
            RCF::Server::Command* c = cg->getNextCommand();
            if(c->getName() == "test1") {
                success = true;
            }
        } catch(RCF::Common::AtEndException& e) {
            break;
        }
    }
    if(!success) {
        cout << "commandsAtEnd(), getNextCommand() 1 failed!" << endl;
        return EXIT_FAILURE;
    }
    success = false;
    while(!subcg->commandsAtEnd()) {
        try {
            RCF::Server::Command* c = subcg->getNextCommand();
            if(c->getName() == "test2") {
                success = true;
            }
        } catch(RCF::Common::AtEndException& e) {
            break;
        }
    }
    if(!success) {
        cout << "commandsAtEnd(), getNextCommand() 2 failed!" << endl;
        return EXIT_FAILURE;
    }
    cg->resetCommandsIterator();
    subcg->resetCommandsIterator();
    success = false;
    while(!cg->commandsAtEnd()) {
        try {
            RCF::Server::Command* c = cg->getNextCommand();
            if(c->getName() == "test1") {
                success = true;
            }
        } catch(RCF::Common::AtEndException& e) {
            break;
        }
    }
    if(!success) {
        cout << "resetCommandsIterator() 1 failed!" << endl;
        return EXIT_FAILURE;
    }
    success = false;
    while(!subcg->commandsAtEnd()) {
        try {
            RCF::Server::Command* c = subcg->getNextCommand();
            if(c->getName() == "test2") {
                success = true;
            }
        } catch(RCF::Common::AtEndException& e) {
            break;
        }
    }
    if(!success) {
        cout << "resetCommandsIterator() 2 failed!" << endl;
        return EXIT_FAILURE;
    }
    success = false;
    while(!cg2->groupsAtEnd()) {
        try {
            RCF::Server::CommandGroup* scg = cg2->getNextGroup();
            if(scg->getName() == "testsubcg") {
                success = true;
            }
        } catch(RCF::Common::AtEndException& e) {
            break;
        }
    }
    if(!success) {
        cout << "groupsAtEnd(), getNextGroup() failed!";
    }
    cg2->resetGroupsIterator();
    success = false;
    while(!cg2->groupsAtEnd()) {
        try {
            RCF::Server::CommandGroup* scg = cg2->getNextGroup();
            if(scg->getName() == "testsubcg") {
                success = true;
            }
        } catch(RCF::Common::AtEndException& e) {
            break;
        }
    }
    if(!success) {
        cout << "resetGroupsIterator failed!";
    }
    try {
        cg2->save();
        subcg->save();
    } catch(RCF::Common::FilesystemException& e) {
        cout << "Already saved..." << endl;
    }
    RCF::Server::CommandGroup* ccg2 = RCF::Server::CommandGroup::load("testcg2");
    success = false;
    while(!ccg2->groupsAtEnd()) {
        try {
            RCF::Server::CommandGroup* scg = ccg2->getNextGroup();
            if(scg->getName() == "testsubcg") {
                success = true;
            }
        } catch(RCF::Common::AtEndException& e) {
            break;
        }
    }
    if(!success) {
        cout << "load() failed - 1!" << endl;
        return EXIT_FAILURE;
    }
    RCF::Server::CommandGroup* csubcg = ccg2->getGroupByName("testsubcg");
    success = false;
    while(!csubcg->commandsAtEnd()) {
        try {
            RCF::Server::Command* c = csubcg->getNextCommand();
            if(c->getName() == "test2") {
                success = true;
            }
        } catch(RCF::Common::AtEndException& e) {
            break;
        }
    }
    if(!success) {
        cout << "load() failed - 2!" << endl;
        return EXIT_FAILURE;
    }
    delete cg;
    delete cg2;
    delete subcg;
    RCF::Server::CommandGroup::free();
    RCF::Server::Command::free();
    RCF::Server::Permission::free();
    cout << "Success!" << endl;
    return EXIT_SUCCESS;
}
