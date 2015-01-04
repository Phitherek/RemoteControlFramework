#include <librcfserver/librcfserver.h>
#include <librcfcommon/librcfcommon.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

int main() {
    RCF::Server::Command* c1 = new RCF::Server::Command("test1", "echo Test successful!");
    RCF::Server::Command* c2 = new RCF::Server::Command("test2", "echo Works {rcfparam:0} and {rcfparam:1}!", 2);
    if(c1->getName() != "test1" || c2->getName() != "test2") {
        cerr << "getName() failed!" << endl;
        return EXIT_FAILURE;
    }
    if(c1->getExec() != "echo Test successful!") {
        cerr << "getExec() 1 failed!" << endl;
        return EXIT_FAILURE;
    }
    try {
        c2->getExec();
    } catch(RCF::Common::ParametersNeededException& e) {
        cout << "Cannot execute parametrized command without parameters - OK!" << endl;
    }
    vector<string> params;
    params.push_back("good");
    if(c1->getExec(params) != "echo Test successful!") {
        cerr << "getExec() 2 failed (with noparam command)" << endl;
        return EXIT_FAILURE;
    }
    try {
        c2->getExec(params);
    } catch(RCF::Common::ParametersNeededException& e) {
        cout << "Cannot execute parametrized command with too few parameters - OK!" << endl;
    }
    params.push_back("better");
    if(c2->getExec(params) != "echo Works good and better!") {
        cerr << "getExec() 2 failed (with parametrized command and sufficient nr of params)" << endl;
        return EXIT_FAILURE;
    }
    params.push_back("best");
    if(c2->getExec(params) != "echo Works good and better!") {
        cerr << "getExec() 2 failed (with parametrized command and too many params)" << endl;
        return EXIT_FAILURE;
    }
    RCF::Server::User* tu = new RCF::Server::User("Tu", "tu");
    RCF::Server::User* tu2 = new RCF::Server::User("Tu2", "tu2");
    RCF::Server::User* tu3 = new RCF::Server::User("Tu3", "tu3");
    RCF::Server::Group* tg = new RCF::Server::Group("Tg");
    tg->addUser(tu2);
    tg->addUser(tu3);
    try {
        tu->save();
        tu2->save();
        tu3->save();
        tg->save();
    } catch(RCF::Common::FilesystemException& e) {
        cout << "Already saved..." << endl;
    }
    c1->addPermission(tu);
    c2->addPermission(tg);
    if(!c1->canExecute("Tu") || !c2->canExecute("Tu2") || !c2->canExecute("Tu3")) {
        cout << "addPermission() or canExecute() failed!" << endl;
        return EXIT_FAILURE;
    }
    try {
        c1->save();
        c2->save();
    } catch(RCF::Common::FilesystemException& e) {
        cout << "Already saved..." << endl;
    }
    RCF::Server::Command* cc1 = RCF::Server::Command::load("test1");
    RCF::Server::Command* cc2 = RCF::Server::Command::load("test2");
    if(cc1->getExec() != c1->getExec()) {
        cerr << "Loading failed: getExec() 1!" << endl;
        return EXIT_FAILURE;
    }
    if(cc2->getExec(params) != c2->getExec(params)) {
        cerr << "Loading failed: getExec() 2!" << endl;
        return EXIT_FAILURE;
    }
    if(cc1->getName() != c1->getName()) {
        cerr << "Loading failed: getName() 1!" << endl;
        return EXIT_FAILURE;
    }
    if(cc2->getName() != c2->getName()) {
        cerr << "Loading failed: getName() 2!" << endl;
        return EXIT_FAILURE;
    }
    if(!cc1->canExecute("Tu")) {
        cerr << "Loading failed: canExecute() 1!" << endl;
        return EXIT_FAILURE;
    }
    if(!cc2->canExecute("Tu2")) {
        cerr << "Loading failed: canExecute() 2!" << endl;
        return EXIT_FAILURE;
    }
    if(!cc2->canExecute("Tu3")) {
        cerr << "Loading failed: canExecute() 3!" << endl;
        return EXIT_FAILURE;
    }
    #if BOOST_OS_LINUX || BOOST_OS_MACOS
        RCF::Server::Command t1("t1", "./testcmd");
        RCF::Server::Command t2("t2", "./testcmdd");
        RCF::Server::Command t3("t3", "./testcmd2 {rcfparam:0} {rcfparam:1}", 2);
    #elif BOOST_OS_WINDOWS
        RCF::Server::Command t1("t1", "testcmd.exe");
        RCF::Server::Command t2("t2", "testcmdd.exe");
        RCF::Server::Command t3("t3", "testcmd2.exe {rcfparam:0} {rcfparam:1}", 2);
    #endif
    RCF::Server::User u("Test", "test123");
    t1.addPermission(&u);
    t2.addPermission(&u);
    t3.addPermission(&u);
    if(!t1.canExecute("Test") || t1.canExecute("blahblah")) {
        cerr << "canExecute() 1 failed!" << endl;
        return EXIT_FAILURE;
    }
    if(!t2.canExecute("Test") || t2.canExecute("blahblah")) {
        cerr << "canExecute() 2 failed!" << endl;
        return EXIT_FAILURE;
    }
    if(!t3.canExecute("Test") || t3.canExecute("blahblah")) {
        cerr << "canExecute() 3 failed!" << endl;
        return EXIT_FAILURE;
    }
    int ret;
    string out = "";
    string err = "";
    try {
        ret = t1.execute(&out, &err);
        #if BOOST_OS_LINUX || BOOST_OS_MACOS
            if(ret != 0 || out != "Stdout test\n" || err != "Stderr test\n") {
                cerr << "execute() 1 failed!" << endl;
                return EXIT_FAILURE;
            }
        #elif BOOST_OS_WINDOWS
            if(ret != 0 || out != "Stdout test\r\n" || err != "Stderr test\r\n") {
                cerr << "execute() 1 failed!" << endl;
                return EXIT_FAILURE;
            }
        #endif
    } catch(RCF::Common::RuntimeException& e) {
        cerr << "execute() 1 failed!" << endl;
        return EXIT_FAILURE;
    }
    out = "";
    err = "";
    try {
        ret = t2.execute(&out, &err);
        #if BOOST_OS_LINUX || BOOST_OS_MACOS
            if(ret != 1 || out != "Stdout test\n" || err != "Stderr test\n") {
                cerr << "execute() 1 failed!" << endl;
                return EXIT_FAILURE;
            }
        #elif BOOST_OS_WINDOWS
            if(ret != 1 || out != "Stdout test\r\n" || err != "Stderr test\r\n") {
                cerr << "execute() 1 failed!" << endl;
                return EXIT_FAILURE;
            }
        #endif
    } catch(RCF::Common::RuntimeException& e) {
        cerr << "execute() 2 failed!" << endl;
        return EXIT_FAILURE;
    }
    out = "";
    err = "";
    try {
        ret = t3.execute(&out, &err, params);
        #if BOOST_OS_LINUX || BOOST_OS_MACOS
            if(ret != 1 || out != "Stdout test: good better\n" || err != "Stderr test\n") {
                cerr << "execute() 1 failed!" << endl;
                return EXIT_FAILURE;
            }
        #elif BOOST_OS_WINDOWS
            if(ret != 1 || out != "Stdout test: good better\r\n" || err != "Stderr test\r\n") {
                cerr << "execute() 1 failed!" << endl;
                return EXIT_FAILURE;
            }
        #endif
    } catch(RCF::Common::RuntimeException& e) {
        cerr << "execute() 3 failed!" << endl;
        return EXIT_FAILURE;
    }
    delete tu;
    delete tu2;
    delete tu3;
    delete tg;
    delete c1;
    delete c2;
    RCF::Server::Permission::free();
    RCF::Server::Command::free();
    cout << "Success!" << endl;
    return EXIT_SUCCESS;
}
