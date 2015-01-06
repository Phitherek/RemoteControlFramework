#include "ServerDefinition.h"
#include <fstream>
using namespace RCF::Client;

ServerDefinition** ServerDefinition::_loadedDefinitions = NULL;
int ServerDefinition::_loadedDefinitionsSize = 0;

ServerDefinition::ServerDefinition(): _certfile("") {
    _name = "";
    _hostname = "";
}

ServerDefinition::ServerDefinition(std::string name, std::string hostname, std::string certfilepath): _certfile(certfilepath) {
    _name = name;
    _hostname = hostname;
    if(!boost::filesystem::exists(_certfile)) {
        throw RCF::Common::FilesystemException(_certfile, "Does not exist!");
    }
}

std::string ServerDefinition::getName() {
    return _name;
}

std::string ServerDefinition::getHostname() {
    return _hostname;
}

boost::filesystem::path ServerDefinition::getCertFile() {
    return _certfile;
}

bool ServerDefinition::valid() {
    if(_name != "" && _hostname != "" && _certfile.string() != "") {
        return true;
    }
    return false;
}

void ServerDefinition::save() {
    if(valid()) {
        RCF::Common::HelperFunctions hf;
        boost::filesystem::path sddir = hf.getHomeDirectory();
        sddir += "/.rcfclient/sdefs";
        boost::filesystem::path filepath = sddir;
        filepath += "/";
        filepath += _name;
        filepath += ".rcfsdef";
        if(!boost::filesystem::exists(sddir)) {
            if(!boost::filesystem::create_directories(sddir)) {
                throw RCF::Common::FilesystemException(sddir, "Could not be created!");
            }
        }
        if(!boost::filesystem::is_directory(sddir)) {
            throw RCF::Common::FilesystemException(sddir, "Is not a directory!");
        }
        if(boost::filesystem::exists(filepath)) {
            throw RCF::Common::FilesystemException(filepath, "Already exists!");
        }
        std::ofstream out(filepath.c_str());
        out << "[name]" << std::endl << _name << std::endl << "[hostname]" << std::endl << _hostname << std::endl << "[certfile]" << std::endl << _certfile.string() << std::endl;
        out.close();
    } else {
        throw RCF::Common::InvalidObjectException("ServerDefinition is invalid and cannot be saved!");
    }
}

ServerDefinition* ServerDefinition::load(std::string name) {
    if(_loadedDefinitions != NULL && _loadedDefinitionsSize > 0) {
        for(int i = 0; i < _loadedDefinitionsSize; i++) {
            if(_loadedDefinitions[i]->getName() == name) {
                return _loadedDefinitions[i];
            }
        }
    }
    RCF::Common::HelperFunctions hf;
    boost::filesystem::path sddir = hf.getHomeDirectory();
    sddir += "/.rcfclient/sdefs";
    boost::filesystem::path filepath = sddir;
    filepath += "/";
    filepath += name;
    filepath += ".rcfsdef";
    if(!boost::filesystem::exists(filepath)) {
        throw RCF::Common::FilesystemException(filepath, "Does not exist!");
    }
    ServerDefinition* sd = new ServerDefinition;
    std::string filepathstr = filepath.string();
    std::ifstream in(filepathstr.c_str());
    std::string ps = "toplevel";
    int lineno = 0;
    while(!in.eof()) {
        std::string line = "";
        char c;
        do {
            c = in.get();
            if(c != '\n' && in) {
                line += c;
            }
        } while(c != '\n' && in);
        if(in) {
            lineno++;
            if(ps == "toplevel") {
                if(line[0] == '[' && line[line.length()-1] == ']') {
                    ps = line.substr(1, line.length()-2);
                } else {
                    throw RCF::Common::ParserException(filepath, lineno, "Expected parameter in [], got " + line + "!");
                }
            } else if(ps == "name")  {
                sd->_name = line;
                ps = "toplevel";
            } else if(ps == "hostname") {
                sd->_hostname = line;
                ps = "toplevel";
            } else if(ps == "certfile") {
                boost::filesystem::path newCertFile(line);
                if(!boost::filesystem::exists(newCertFile)) {
                    throw RCF::Common::FilesystemException(newCertFile, "Does not exist!");
                }
                sd->_certfile = newCertFile;
            } else {
                throw RCF::Common::ParserException(filepath, lineno, "Unknown parameter: " + line + "!");
            }
        }
    }
    in.close();
    int newLoadedDefinitionsSize = _loadedDefinitionsSize + 1;
    ServerDefinition** newLoadedDefinitions = new ServerDefinition*[newLoadedDefinitionsSize];
    if(_loadedDefinitions != NULL && _loadedDefinitionsSize > 0) {
        for(int i = 0; i < _loadedDefinitionsSize; i++) {
            newLoadedDefinitions[i] = _loadedDefinitions[i];
        }
        delete[] _loadedDefinitions;
    }
    newLoadedDefinitions[newLoadedDefinitionsSize-1] = sd;
    _loadedDefinitions = newLoadedDefinitions;
    _loadedDefinitionsSize = newLoadedDefinitionsSize;
    return sd;
}

void ServerDefinition::free() {
    if(_loadedDefinitions != NULL && _loadedDefinitionsSize > 0) {
        for(int i = 0; i < _loadedDefinitionsSize; i++) {
            delete _loadedDefinitions[i];
        }
        delete[] _loadedDefinitions;
    }
    _loadedDefinitions = NULL;
    _loadedDefinitionsSize = 0;
}
