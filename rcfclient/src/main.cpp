#include <boost/predef.h>
#if BOOST_OS_WINDOWS
    #define _WIN32_WINNT 0x0501
    #define WINVER 0x0501
#endif
#include <boost/asio.hpp>
#include <librcfcommon/librcfcommon.h>
#include <librcfclient/librcfclient.h>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <string>
#include <sstream>
using namespace std;

std::string paramProvider() {
    cout << "Enter additional parameter for the command: ";
    std::string param;
    cin >> param;
    return param;
}

int main(int argc, char** argv) {
    if(argc != 2)  {
        cout << "Usage: " << argv[0] << " server_definition_name" << endl;
    } else {
        try {
            cout << "RemoteControlFramework Command-Line Interface Client (C) 2015 by Phitherek_" << endl;
            std::string sdname = "";
            std::string username = "";
            std::string password = "";
            RCF::Common::HelperFunctions hf;
            sdname += argv[1];
            boost::asio::io_service service;
            cout << "Connecting to server defined by: " << sdname << endl;
            RCF::Client::Client cli(service, sdname);
            cout << "Connected to: " << cli.getServerDefinition()->getHostname() << "!" << endl;
            cli.handshake();
            cout << "Successfully handshaked with the server!" << endl;
            cout << "Login as: ";
            cin >> username;
            cout << "Enter password (will not be echoed): ";
            hf.setInputEcho(false);
            cin >> password;
            hf.setInputEcho(true);
            cout << endl;
            cli.authorize(username, password);
            cout << "Authorization successful!" << endl;
            char action = 0;
            do {
                cout << "What do you want to do, " << username << "?" << endl;
                cout << " * display the (l)ist of available commands" << endl;
                cout << " * (e)xecute a command" << endl;
                cout << " * (q)uit from the application" << endl;
                cout << ": ";
                cin >> action;
                if(action == 'l') {
                    std::string cmdlist;
                    cmdlist = cli.list();
                    cout << "List of available commands (-> means executable command, all others are just category names):" << endl << endl;
                    std::string line = "";
                    int indentWidth = 0;
                    int cmdlistLength = cmdlist.length();
                    for(int i = 0; i < cmdlistLength; i++) {
                        if(cmdlist[i] == '\n' || cmdlist[i] == '\r') {
                            if(line != "") {
                                if(line[0] == '[' && line[line.length()-1] == ']') {
                                    if(line.substr(1, line.length()-2) == "end") {
                                        indentWidth -= 2;
                                    } else {
                                        for(int j = 0; j < indentWidth; j++) {
                                            cout << " ";
                                        }
                                        cout << line.substr(1, line.length()-2) << ":" << endl;
                                        indentWidth += 2;
                                    }
                                } else {
                                    for(int j = 0; j < indentWidth; j++) {
                                        cout << " ";
                                    }
                                    cout << "-> " << line  << endl;
                                }
                                line = "";
                            }
                        } else {
                            line += cmdlist[i];
                        }
                    }
                } else if(action == 'e') {
                    cout << "Format of the query is as follows: command name preceded by category names, separated by : sign, as in category:category:command." << endl;
                    cout << "Enter command query: ";
                    std::string query;
                    std::string out = "";
                    std::string err = "";
                    int code;
                    cin >> query;
                    try {
                        cout << "Executing command..." << endl;
                        code = cli.execute(query, &out, &err, paramProvider);
                        cout << "Command finished with exit code: " << code << endl;
                        cout << "Command' s standard console output:" << endl;
                        cout << out << endl;
                        cout << "Command' s standard error output:" << endl;
                        cout << err << endl;
                    } catch(RCF::Common::NotFoundException& e) {
                        cout << "Query could not be resolved!" << endl;
                    } catch(exception& e) {
                        throw e;
                    }
                }
            } while(action != 'q');
            cout << "Closing the connection..." << endl;
            cli.close();
            cout << "Connection closed successfully! Thank you for using RemoteControlFramework!" << endl;
        } catch(RCF::Common::ProtocolException& e) {
            cerr << "Protocol error has occured: " << e.what() << " - connection has been destroyed!" << endl;
            return EXIT_FAILURE;
        } catch(exception& e) {
            cerr << "Caught exception: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
