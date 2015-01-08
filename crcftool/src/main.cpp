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
#include <librcfclient/librcfclient.h>
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
    if(action != "addserver" && action != "listservers" && action != "showserver" && action != "help") {
        action = "help";
    }
    cout << "Client RCFTool v. 0.1  (C) 2015 by Phitherek_" << endl;
    if(action == "help") {
        cout << "Usage: " << arguments[0] << " command" << endl;
        cout << endl << "Available commands:" << endl << endl;
        cout << "addserver - Creates new server definition"  << endl;
        cout << "listservers - List available server definitions" << endl;
        cout << "showserver - Shows content of specified server definition" << endl;
        cout << "help - This help message" << endl;
    } else if(action == "addserver") {
        std::string name;
        std::string hostname;
        std::string certpath;
        cout << "Enter new server definition name: ";
        cin >> name;
        cout << "Enter server address/hostname: ";
        cin >> hostname;
        cout << "Enter path to server SSL certificate file: ";
        cin >> certpath;
        try {
            RCF::Client::ServerDefinition sd(name, hostname, certpath);
            sd.save();
            cout << "New server definition created successfully!" << endl;
        } catch(exception& e) {
            cout << "Caught exception while trying to create new server definition: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    } else if(action == "listservers") {
        RCF::Common::HelperFunctions hf;
        boost::filesystem::path sdpath = hf.getHomeDirectory();
        sdpath += "/.rcfclient/sdefs";
        if(!boost::filesystem::exists(sdpath)) {
            cout << "No server definitions yet..." << endl;
        } else {
            boost::filesystem::directory_iterator sditer(sdpath);
            boost::filesystem::directory_iterator enditer;
            vector<string> sds;
            while(sditer != enditer) {
                if((*sditer).path().extension().string() == ".rcfsdef") {
                    sds.push_back((*sditer).path().stem().string());
                }
                sditer++;
            }
            int sdsSize = sds.size();
            if(sdsSize == 0) {
                cout << "No server definitions yet..." << endl;
            } else {
                cout << "Available server definitions:" << endl;
                for(int i = 0; i < sdsSize; i++) {
                    cout << sds[i] << endl;
                }
            }
        }
    } else if(action == "showserver") {
        cout << "Enter server definition name: ";
        std::string name;
        cin >> name;
        try {
            RCF::Client::ServerDefinition* sd  = RCF::Client::ServerDefinition::load(name);
            cout << "Server definition contains:" << endl;
            cout << "Name: " << sd->getName() << endl;
            cout << "Server address/hostname: " << sd->getHostname() << endl;
            cout << "Server certificate file path: " << sd->getCertFile().string() << endl;
        } catch(exception& e) {
            cout << "Caught exception while trying to display server definition: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
