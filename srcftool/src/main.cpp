#include <boost/predef.h>
#if BOOST_OS_WINDOWS
    #define _WIN32_WINNT 0x0501
    #define WINVER 0x0501
#endif
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <exception>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <librcfserver/librcfserver.h>
#include <librcfcommon/librcfcommon.h>
using namespace std;

int main(int argc, char** argv) {
    vector<string> arguments;
    string action = "";
    for(int i = 0; i < argc; i++) {
        arguments.push_back(argv[i]);
    }
    if(argc != 2) {
        action = "help";
    } else {
        action = arguments[1];
    }
    if(action != "adduser" && action != "addgroup" && action != "addcommand" && action != "addcommandgroup" && action != "help") {
        action = "help";
    }
    cout << "Server RCFTool v. 0.1 (C) 2015 by Phitherek_" << endl;
    if(action == "help") {
        cout << "Usage: " << arguments[0] << " command" << endl;
        cout << endl << "Available commands:" << endl << endl;
        cout << "adduser - Creates new user"  << endl;
        cout << "addgroup - Creates new empty group" << endl;
        cout << "addcommand - Creates new command (without permissions)" << endl;
        cout << "addcommandgroup - Creates new empty command group" << endl;
        cout << "help - This help message" << endl;
    } else if(action == "adduser") {
        RCF::Common::HelperFunctions hf;
        std::string name;
        std::string password;
        std::string password_confirmation;
        cout << "Enter new user name: ";
        cin >> name;
        cout << "Enter new user password (will not be echoed): ";
        hf.setInputEcho(false);
        cin >> password;
        hf.setInputEcho(true);
        cout << endl;
        cout << "Confirm new user password (will not be echoed): ";
        hf.setInputEcho(false);
        cin >> password_confirmation;
        hf.setInputEcho(true);
        cout << endl;
        if(password != password_confirmation) {
            cout << "Password and confirmation do not match!" << endl;
            return EXIT_FAILURE;
        }
        try {
            RCF::Server::User u(name, password);
            u.save();
            cout << "Success!" << endl;
        } catch(exception& e) {
            cout << "Exception caught while trying to create new user: " << e.what() << endl;
        }
    } else if(action == "addgroup") {
        std::string name;
        cout << "Enter group name: ";
        cin >> name;
        try {
            RCF::Server::Group g(name);
            g.save();
            cout << "Success!" << endl;
        } catch(exception& e) {
            cout << "Exception caught while trying to create new group: " << e.what() << endl;
        }
    } else if(action == "addcommand") {
        std::string name;
        std::string exec;
        int numParams;
        cout << "Enter new command name: ";
        cin >> name;
        cout << "Enter new command exec string (use {rcfparam:number} to indicate parameters): ";
        exec = "";
        char c;
        cin.ignore();
        do {
           c = cin.get();
           if(c != '\n' && c != '\r') {
               exec += c;
           }
        } while(c  != '\n' && c != '\r');
        cout << "Enter number of new command parameters: ";
        cin >> numParams;
        try {
            if(numParams == 0) {
                RCF::Server::Command c(name, exec);
                c.save();
                cout << "Success!" << endl;
            } else if(numParams > 0) {
                RCF::Server::Command c(name, exec, numParams);
                c.save();
                cout << "Success!" << endl;
            } else {
                cout << "Number of parameters must be positive!" << endl;
                return EXIT_FAILURE;
            }
        } catch(exception& e) {
            cout << "Exception caught while trying to create new command: " << e.what() << endl;
        }
    } else if(action == "addcommandgroup") {
        std::string name;
        cout << "Enter new command group name: ";
        cin >> name;
        try {
            RCF::Server::CommandGroup cg(name);
            cg.save();
            cout << "Success!" << endl;
        }  catch(exception& e) {
            cout << "Exception caught while trying to create new command group: " << e.what() << endl;
        }
    }
    return EXIT_SUCCESS;
}
