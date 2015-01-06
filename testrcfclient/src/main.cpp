#include <librcfcommon/librcfcommon.h>
#include <librcfclient/librcfclient.h>
#include <boost/asio.hpp>
#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char** argv) {
    if(argc != 2)  {
        cout << "Usage: " << argv[0] << " server_definition_name" << endl;
    } else {
        cout << "RemoteControlFramework Test Client (C) 2015 by Phitherek_" << endl;
        cout << "Used ONLY for server testing purposes, do not use as regular client!" << endl;
        std::string sdname = "";
        sdname += argv[1];
        boost::asio::io_service service;
        RCF::Client::Client cli(service, sdname);
        std::string line = "";
        do {
            char c = cin.get();
            if(c == '\n') {
                if(line != "QUIT") {
                    if(line != "NEXT") {
                        cli.write(line);
                    }
                    std::string outline = cli.read();
                    cout << outline << endl;
                    line = "";
                } else {
                    break;
                }
            } else {
                line += c;
            }
        } while(line != "QUIT");
    }
    return EXIT_SUCCESS;
}
