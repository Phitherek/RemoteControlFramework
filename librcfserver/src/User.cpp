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
    
}

void User::save() {
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
            // Could not create permission config directory
        }
    }
    if(!boost::filesystem::is_directory(permdir)) {
        // Permission config directory is not a directory...
    }
    if(boost::filesystem::exists(filepath)) {
        // User already exists
    }
    std::string filepathstr = filepath.string();
    std::ofstream out(filepathstr.c_str());
    out << "[name]" << std::endl << _name << std::endl << "[salt]" << std::endl << _salt << std::endl << "[hashed_password]" << std::endl << _hashed_password << std::endl;
    out.close();
}

std::string User::getPermissionType() {
    return "user";
}

bool User::checkPassword(std::string password) {
    if(!valid()) {
        return false;
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
