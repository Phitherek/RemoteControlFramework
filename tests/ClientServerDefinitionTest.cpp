#include <librcfcommon/librcfcommon.h>
#include <librcfclient/librcfclient.h>
#include <iostream>
#include <cstdlib>
#include <boost/filesystem.hpp>
using namespace std;

int main() {
    RCF::Common::HelperFunctions hf;
    boost::filesystem::path certfile = hf.getHomeDirectory();
    certfile += "/rcfcerts/localtest/rcfservercert.pem";
    RCF::Client::ServerDefinition sd("localtest", "localhost", certfile.string());
    if(sd.getName() != "localtest") {
        cerr << "getName() failed!" << endl;
        return EXIT_FAILURE;
    }
    if(sd.getHostname() != "localhost") {
        cerr << "getHostname() failed!" << endl;
        return EXIT_FAILURE;
    }
    if(sd.getCertFile().string() != certfile.string()) {
        cerr << "getCertFile() failed!" << endl;
        return EXIT_FAILURE;
    }
    try {
        sd.save();
    } catch(RCF::Common::FilesystemException& e) {
        cout << "Already saved..." << endl;
    }
    RCF::Client::ServerDefinition* csd = RCF::Client::ServerDefinition::load("localtest");
    if(csd->getName() != sd.getName() || csd->getHostname() != sd.getHostname() || csd->getCertFile().string() != sd.getCertFile().string()) {
        cerr << "load() failed!" << endl;
        RCF::Client::ServerDefinition::free();
        return EXIT_FAILURE;
    }
    RCF::Client::ServerDefinition::free();
    cout << "Success!" << endl;
    return EXIT_SUCCESS;
}
