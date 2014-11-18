#include "User.h"
#include <librcfcommon/librcfcommon.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
using namespace RCF::Server;

bool User::valid() {
    if(_name != "" && _salt != "" && _hashed_password != "") {
        return true;
    }
    return false;
}

User::User() {
    _name = "";
    _salt = "";
    _hashed_password = "";
}

User::User(std::string name, std::string password) {
    _name = name;
    RCF::Common::MHashEngine mhe;
    std::stringstream saltbasess;
    saltbasess.str("");
    saltbasess << time(NULL) << "--" << password;
    _salt  = mhe.getHash(saltbasess.str());
    std::stringstream hpss;
    hpss.str("");
    hpss << _salt << "--" << password;
    _hashed_password = mhe.getHash(hpss.str());
}

void User::load(std::string name) {
    RCF::Common::HelperFunctions hf;
    boost::filesystem::path confdir = hf.getHomeDirectory();
    confdir += "/.rcfserver";
    boost::filesystem::path permdir = confdir;
    permdir += "/perms";
    boost::filesystem::path filepath = permdir;
    filepath += "/";
    filepath += name;
    filepath += ".rcfuser";
    if(!boost::filesystem::exists(filepath)) {
        throw RCF::Common::FilesystemException(filepath, "Does not exist!");
    }
    std::string filepathstr = filepath.string();
    std::ifstream in(filepathstr.c_str());
    std::string ps = "toplevel";
    int lineno = 0;
    while(!in.eof()) {
        std::string line = "";
        in >> line;
        if(in) {
            lineno++;
            if(ps == "toplevel") {
                if(line[0] == '[' && line[line.length()-1] == ']') {
                    ps = line.substr(1, line.length()-2);
                } else {
                    throw RCF::Common::ParserException(filepath, lineno, "Expected parameter in [], got " + line + "!");
                }
            } else if(ps == "name") {
                _name = line;
                ps = "toplevel";
            } else if(ps == "salt") {
                _salt = line;
                ps = "toplevel";
            } else if(ps == "hashed_password") {
                _hashed_password = line;
                ps = "toplevel";
            } else {
                throw RCF::Common::ParserException(filepath, lineno, "Unknown parameter: " + line + "!");
            }
        }
    }
    in.close();
}

void User::save() {
    if(valid()) {
        RCF::Common::HelperFunctions hf;
        boost::filesystem::path confdir = hf.getHomeDirectory();
        confdir += "/.rcfserver";
        boost::filesystem::path permdir = confdir;
        permdir += "/perms";
        boost::filesystem::path filepath = permdir;
        filepath += "/";
        filepath += _name;
        filepath += ".rcfuser";
        if(!boost::filesystem::exists(permdir)) {
            if(!boost::filesystem::create_directories(permdir)) {
                throw RCF::Common::FilesystemException(permdir, "Could not be created!");
            }
        }
        if(!boost::filesystem::is_directory(permdir)) {
            throw RCF::Common::FilesystemException(permdir, "Is not a directory!");
        }
        if(boost::filesystem::exists(filepath)) {
            throw RCF::Common::FilesystemException(filepath, "Already exists!");
        }
        std::string filepathstr = filepath.string();
        std::ofstream out(filepathstr.c_str());
        out << "[name]" << std::endl << _name << std::endl << "[salt]" << std::endl << _salt << std::endl << "[hashed_password]" << std::endl << _hashed_password << std::endl;
        out.close();
    } else {
        throw RCF::Common::InvalidObjectException("User is invalid and cannot be saved!");
    }
}

std::string User::getPermissionType() {
    return "user";
}

bool User::checkPassword(std::string password) {
    if(!valid()) {
        throw RCF::Common::InvalidObjectException("User is invalid, cannot check password!");
    }
    RCF::Common::MHashEngine mhe;
    std::stringstream checkss;
    checkss.str("");
    checkss << _salt << "--" << password;
    if(mhe.getHash(checkss.str()) == _hashed_password) {
        return true;
    }
    return false;
}
