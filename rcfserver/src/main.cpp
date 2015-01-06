#include <librcfserver/librcfserver.h>
#include <librcfcommon/librcfcommon.h>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <cstdlib>
#include <string>
#include <exception>
using namespace std;

void mainfn(RCF::Server::Server* srv, boost::asio::ssl::stream<boost::asio::ip::tcp::socket>* sock) {
    try {
        srv->handshake(sock);
        cout << "Client connected!" << endl;
        RCF::Server::User* logged_user = srv->authorize(sock);
        srv->handlePostauth(sock, logged_user);
    } catch(exception& e) {
        srv->handleException(sock, e);
        return;
    }
}

int main() {
    cout << "RemoteControlFramework Server v. 0.1 (c) 2015 by Phitherek_" << endl;
    cout << "Loading configuration..." << endl;
    try {
        boost::asio::io_service service;
        RCF::Server::Server srv(service, 8910, mainfn);
        cout << "Starting server on port 8910..." << endl;
        srv.start();
        cout << "Cleaning up..." << endl;
    } catch(exception& e) {
        cout << "Caught exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
