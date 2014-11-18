#include "Group.h"
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
#include <librcfcommon/librcfcommon.h>
using namespace RCF::Server;

Group::Group() {
    _name = "";
    _users = NULL;
    _usersSize = 0;
    _usersIterator = -1;
    _usersEnd = true;
}

Group::Group(std::string name) {
    _name = name;
    _users = NULL;
    _usersSize = 0;
    _usersIterator = -1;
    _usersEnd = true;
}

Group::Group(Group& cpy) {
    _name = cpy._name;
    if(cpy._users == NULL && cpy._usersSize == 0) {
        _users = cpy._users;
    } else {
        _users = new User[cpy._usersSize];
        for(int i = 0; i < cpy._usersSize; i++) {
            _users[i] = cpy._users[i];
        }
    }
    _usersSize = cpy._usersSize;
    _usersIterator = -1;
    setUsersEnd();
}

bool Group::valid() {
    if(_name != "") {
        return true;
    }
    return false;
}

void setUsersEnd() {
    if(_usersSize == 0 || _usersIterator >= _usersSize-1) {
        _usersEnd = true;
    } else {
        _usersEnd = false;
    }
}

void Group::load() {
    RCF::Common::HelperFunctions hf;
    boost::filesystem::path confdir = hf.getHomeDirectory();
    confdir += "/.rcfserver";
    boost::filesystem::path permdir = confdir;
    permdir += "/perms";
    boost::filesystem::path filepath = permdir;
    filepath += "/";
    filepath += name;
    filepath += ".rcfgroup";
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
            } else if(ps == "users") {
                if(line[0] == '[' && line[line.length()-1] == ']') {
                    if(line.substr(1, line.length()-2) == "usersend") {
                        ps = "toplevel";
                    } else {
                        throw RCF::Common::ParserException(filepath, lineno, "Expected [usersend] or username, got " + line + "!");
                    }
                } else {
                    User u;
                    u.load(line);
                    addUser(u);
                }
            } else {
                throw RCF::Common::ParserException(filepath, lineno, "Unknown parameter: " + line + "!");
            }
        }
    }
    in.close();
}

void Group::save() {
    if(valid()) {
        RCF::Common::HelperFunctions hf;
        boost::filesystem::path confdir = hf.getHomeDirectory();
        confdir += "/.rcfserver";
        boost::filesystem::path permdir = confdir;
        permdir += "/perms";
        boost::filesystem::path filepath = permdir;
        filepath += "/";
        filepath += _name;
        filepath += ".rcfgroup";
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
        out << "[name]" << std::endl << _name << std::endl;
        if(_usersSize > 0) {
            out << "[users]" << std::endl;
            for(int i = 0; i < _usersSize; i++) {
                out << _users[i].getName() << std::endl;
            }
            out << "[usersend]" << std::endl;
        }
        out.close();
    } else {
        throw RCF::Common::InvalidObjectException("Group is invalid and cannot be saved!");
    }
}

std::string Group::getPermissionType() {
    return "group";
}

void Group::addUser(User u) {
    for(int i = 0; i < _usersSize; i++) {
        if(_users[i].getName() == u.getName()) {
            throw RCF::Common::AlreadyExistsException(u.getName(), "User already exists in group!");
        }
    }
    if(u.valid()) {
        int newSize = _usersSize+1;
        User* newUsers  = new User[newSize];
        if(_users != NULL) {
            for(int i = 0; i < _usersSize; i++) {
                newUsers[i] = _users[i];
            }
            delete[] _users;
            _users = NULL;
        }
        newUsers[newSize-1] = u;
        _users = newUsers;
        _usersSize = newSize;
    } else {
        throw RCF::Common::InvalidObjectException("User is invalid and cannot be added to group!");
    }
}

void Group::removeUser(std::string username) {
    int torem = -1;
    for(int i = 0; i < _usersSize; i++) {
        if(_users[i].getName() == username) {
            torem = i;
            break;
        }
    }
    if(torem == -1) {
        throw RCF::Common::NotFoundException(username, "User could not be found and cannot be removed from the group!");
    }
    int newSize = _usersSize - 1;
    User* newUsers = new User[newSize];
    for(int i = 0; i < torem; i++) {
        newUsers[i] = _users[i];
    }
    for(int i = torem+1; i < _usersSize; i++) {
        newUsers[i-1] = _users[i];
    }
    delete[] _users;
    _users = newUsers;
    _usersSize = newSize;
    if(_usersIterator >= torem) {
        _usersIterator = _usersIterator-1;
    }
    setUsersEnd();
}

bool Group::hasUser(std::string username) {
    for(int i = 0; i < _usersSize; i++) {
        if(_users[i].getName() == username) {
            return true;
        }
    }
    return false;
}

User Group::getNextUser() {
    if(_usersIterator < _usersSize-1) {
        _usersIterator++;
        setUsersEnd();
        return _users[_usersIterator];
    } else {
        setUsersEnd();
        throw RCF::Common::AtEndException("Could not get next user, iterator at the end of users collection!");
    }
}

bool Group::usersAtEnd() {
    setUsersEnd();
    return _usersEnd;
}

void Group::resetUsersIterator() {
    _usersIterator = -1;
}
