#include "Command.h"
#include <librcfcommon/librcfcommon.h>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
using namespace RCF::Server;

Command::Command(std::string name, std::string exec) {
    _name = name;
    _exec = exec;
    _perms = NULL;
    _permsSize = 0;
    _permsIterator = -1;
    _permsEnd = true;
    _numParams = 0;
}

Command::Command(std::string name, std::string exec, int numParams) {
    _name = name;
    _exec = exec;
    _perms = NULL;
    _permsSize = 0;
    _permsIterator = -1;
    _permsEnd = true;
    _numParams = numParams;
}

void Command::setPermsEnd() {
    if(_permsSize == 0 || _permsIterator >= _permsSize-1) {
        _permsEnd = true;
    } else {
        _permsEnd = false;
    }
}

std::string Command::getName() {
    return _name;
}

std::string Command::getExec() {
    if(_numParams == 0) {
        return _exec;
    } else {
        throw RCF::Common::ParametersNeededException("Insufficient parameters for " + _name + " command!", _numParams, 0);
    }
}

std::string Command::getExec(std::vector<std::string> params) {
    int paramsSize = params.size();
    if(paramsSize < _numParams) {
        throw RCF::Common::ParametersNeededException("Insufficient parameters for " + _name + " command!", _numParams, params.size());
    } else {
        std::string potentialparam = "";
        std::string pdstatus = "initial";
        std::string out = "";
        int execLength = _exec.length();
        for(int i = 0; i < execLength; i++) {
            if(pdstatus == "initial") {
                potentialparam = "";
                if(_exec[i] == '{') {
                    pdstatus = "potentialparam-0";
                    potentialparam += _exec[i];
                } else {
                    out += _exec[i];
                }
            } else if(pdstatus == "potentialparam-0") {
                if(_exec[i] == 'r') {
                    pdstatus = "potentialparam-1";
                    potentialparam += _exec[i];
                } else {
                    out += potentialparam;
                    out += _exec[i];
                    pdstatus = "initial";
                }
            } else if(pdstatus == "potentialparam-1") {
                if(_exec[i] == 'c') {
                    pdstatus = "potentialparam-2";
                    potentialparam += _exec[i];
                } else {
                    out += potentialparam;
                    out += _exec[i];
                    pdstatus = "initial";
                }
            } else if(pdstatus == "potentialparam-2") {
                if(_exec[i] == 'f') {
                    pdstatus = "potentialparam-3";
                    potentialparam += _exec[i];
                } else {
                    out += potentialparam;
                    out += _exec[i];
                    pdstatus = "initial";
                }
            } else if(pdstatus == "potentialparam-3") {
                if(_exec[i] == 'p') {
                    pdstatus = "potentialparam-4";
                    potentialparam += _exec[i];
                } else {
                    out += potentialparam;
                    out += _exec[i];
                    pdstatus = "initial";
                }
            } else if(pdstatus == "potentialparam-4") {
                if(_exec[i] == 'a') {
                    pdstatus = "potentialparam-5";
                    potentialparam += _exec[i];
                } else {
                    out += potentialparam;
                    out += _exec[i];
                    pdstatus = "initial";
                }
            } else if(pdstatus == "potentialparam-5") {
                if(_exec[i] == 'r') {
                    pdstatus = "potentialparam-6";
                    potentialparam += _exec[i];
                } else {
                    out += potentialparam;
                    out += _exec[i];
                    pdstatus = "initial";
                }
            } else if(pdstatus == "potentialparam-6") {
                if(_exec[i] == 'a') {
                    pdstatus = "potentialparam-7";
                    potentialparam += _exec[i];
                } else {
                    out += potentialparam;
                    out += _exec[i];
                    pdstatus = "initial";
                }
            } else if(pdstatus == "potentialparam-7") {
                if(_exec[i] == 'm') {
                    pdstatus = "potentialparam-8";
                    potentialparam += _exec[i];
                } else {
                    out += potentialparam;
                    out += _exec[i];
                    pdstatus = "initial";
                }
            } else if(pdstatus == "potentialparam-8") {
                if(_exec[i] == ':') {
                    pdstatus = "parsenum";
                    potentialparam += _exec[i];
                } else {
                    out += potentialparam;
                    out += _exec[i];
                    pdstatus = "initial";
                }
            } else if(pdstatus == "parsenum") {
                if(_exec[i] == '}') {
                    potentialparam  = potentialparam.substr(1, potentialparam.length()-1);
                    std::string paramnum = "";
                    bool parsenum = false;
                    int potentialparamLength = potentialparam.length();
                    for(int j = 0; j < potentialparamLength; j++) {
                        if(!parsenum && potentialparam[j] == ':') {
                            parsenum = true;
                        } else if(parsenum) {
                            paramnum += potentialparam[j];
                        }
                    }
                    int ipn = atoi(paramnum.c_str());
                    if(ipn >= _numParams) {
                        throw RCF::Common::ParametersNeededException("Invalid parameter specification for " + _name + " command!", _numParams, ipn);
                    } else {
                        out += params[ipn];
                    }
                    potentialparam = "";
                    pdstatus = "initial";
                } else {
                    potentialparam += _exec[i];
                }
            }
        }
        out += potentialparam;
        return out;
    }
}

void Command::addPermission(Permission* p) {
    if(p->valid()) {
        for(int i = 0; i < _permsSize; i++)  {
            if(_perms[i] == p) {
                throw RCF::Common::AlreadyExistsException(_name, "Permission already exists in this command!");
            }
        }
        int newPermsSize = _permsSize+1;
        Permission** newPerms = new Permission*[newPermsSize];
        if(_perms != NULL) {
            for(int i = 0; i < _permsSize; i++) {
                newPerms[i] = _perms[i];
            }
            delete[] _perms;
        }
        newPerms[newPermsSize-1] = p;
        _perms = newPerms;
        _permsSize = newPermsSize;
        setPermsEnd();
    } else {
        throw RCF::Common::InvalidObjectException("Cannot add invalid permission to command!");
    }
}

Permission* Command::getNextPermission() {
    if(_permsIterator < _permsSize-1) {
        _permsIterator++;
        setPermsEnd();
        return _perms[_permsIterator];
    } else {
        setPermsEnd();
        throw RCF::Common::AtEndException("Could not get next permission, iterator at the end of permissions collection!");
    }
}

bool Command::permissionsAtEnd() {
    setPermsEnd();
    return _permsEnd;
}

void Command::resetPermissionIterator() {
    _permsIterator = -1;
    setPermsEnd();
}

int Command::execute(std::string* output, std::string* error) {
    std::string exec = getExec();
    return ExecuteImpl::execute(exec, output, error);
}

int Command::execute(std::string* output, std::string* error, std::vector<std::string> params) {
    std::string exec = getExec();
    return ExecuteImpl::execute(exec, output, error);
}

bool Command::canExecute(std::string username) {
    for(int i = 0; i < _permsSize; i++) {
        if((_perms[i]->getPermissionType() == "user" && _perms[i]->getName() == username) || (_perms[i]->getPermissionType() == "group" && dynamic_cast<Group*>(_perms[i])->hasUser(username))) {
           return true;
        }
    }
    return false;
}

void Command::save() {
    if(valid()) {
        RCF::Common::HelperFunctions hf;
        boost::filesystem::path confdir = hf.getHomeDirectory();
        confdir += "/.rcfserver";
        boost::filesystem::path cmddir = confdir;
        cmddir += "/commands";
        boost::filesystem::path filepath = cmddir;
        filepath += "/";
        filepath += _name;
        filepath += ".rcfcmd";
        if(!boost::filesystem::exists(cmddir)) {
            if(!boost::filesystem::create_directories(cmddir)) {
                throw RCF::Common::FilesystemException(cmddir, "Could not be created!");
            }
        }
        if(!boost::filesystem::is_directory(cmddir)) {
            throw RCF::Common::FilesystemException(cmddir, "Is not a directory!");
        }
        if(boost::filesystem::exists(filepath)) {
            throw RCF::Common::FilesystemException(filepath, "Already exists!");
        }
        std::string filepathstr = filepath.string();
        std::ofstream out(filepathstr.c_str());
        out << "[name]" << std::endl << _name << std::endl << "[exec]" << std::endl << _exec << std::endl << "[num_params]" << std::endl << _numParams << std::endl;
        out << "[users]" << std::endl;
        for(int i = 0; i < _permsSize; i++) {
            if(_perms[i]->getPermissionType() == "user") {
                out << _perms[i]->getName() << std::endl;
            }
        }
        out << "[usersend]" << std::endl  << "[groups]" << std::endl;
        for(int i = 0; i < _permsSize; i++) {
            if(_perms[i]->getPermissionType() == "group") {
                out << _perms[i]->getName() << std::endl;
            }
        }
        out << "[groupsend]" << std::endl;
    } else {
        throw RCF::Common::InvalidObjectException("Command is invalid and cannot be saved!");
    }
}

void Command::load(std::string name) {
    RCF::Common::HelperFunctions hf;
    boost::filesystem::path confdir = hf.getHomeDirectory();
    confdir += "/.rcfserver";
    boost::filesystem::path cmddir = confdir;
    cmddir += "/commands";
    boost::filesystem::path filepath = cmddir;
    filepath += "/";
    filepath += _name;
    filepath += ".rcfcmd";
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
            } else if(ps == "name")  {
                _name = line;
                ps = "toplevel";
            } else if(ps == "exec") {
                _exec = line;
                ps = "toplevel";
            } else if(ps == "num_params") {
                _numParams = atoi(line.c_str());
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
                    addPermission(&u);
                }
            } else if(ps == "groups") {
                if(line[0] == '[' && line[line.length()-1] == ']') {
                    if(line.substr(1, line.length()-2) == "groupsend") {
                        ps = "toplevel";
                    } else {
                        throw RCF::Common::ParserException(filepath, lineno, "Expected [groupsend] or username, got " + line + "!");
                    }
                } else {
                    Group g;
                    g.load(line);
                    addPermission(&g);
                }
            } else {
                throw RCF::Common::ParserException(filepath, lineno, "Unknown parameter: " + line + "!");
            }
        }
    }
}

bool Command::valid() {
    return (_name != "" && _exec != "");
}

int Command::numParams() {
    return _numParams;
}
