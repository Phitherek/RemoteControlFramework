#include "User.h"
#include <librcfcommon/librcfcommon.h>
#include <ctime>
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
