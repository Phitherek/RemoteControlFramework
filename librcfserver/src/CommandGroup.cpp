#include "CommandGroup.h"
using namespace RCF::Server;

CommandGroup** CommandGroup::_loadedGroups = NULL;
int CommandGroup::_loadedGroupsSize = 0;

void CommandGroup::setCommandsEnd() {
    if(_commands != NULL && _commandsIterator < _commandsSize-1) {
        _commandsEnd = false;
    } else {
        _commandsEnd = true;
    }
}

void CommandGroup::setGroupsEnd() {
    if(_groups != NULL && _groupsIterator < _groupsSize-1) {
        _groupsEnd = false;
    } else {
        _groupsEnd = true;
    }
}

CommandGroup::CommandGroup() {
    _name = "";
    _commands = NULL;
    _commandsSize = 0;
    _commandsIterator = -1;
    _commandsEnd = true;
    _groups = NULL;
    _groupsSize = 0;
    _groupsIterator = -1;
    _groupsEnd = true;
}

CommandGroup::CommandGroup(std::string name) {
    _name = name;
    _commands = NULL;
    _commandsSize = 0;
    _commandsIterator = -1;
    _commandsEnd = true;
    _groups = NULL;
    _groupsSize = 0;
    _groupsIterator = -1;
    _groupsEnd = true;
}

CommandGroup::CommandGroup(CommandGroup& cpy) {
    _name = cpy._name;
    if(cpy._commands != NULL && cpy._commandsSize > 0) {
        _commands = new Command*[cpy._commandsSize];
        for(int i = 0; i < cpy._commandsSize; i++) {
            _commands[i] = cpy._commands[i];
        }
        _commandsSize = cpy._commandsSize;
    } else {
        _commands = NULL;
        _commandsSize = 0;
    }
    _commandsIterator = -1;
    setCommandsEnd();
    if(cpy._groups != NULL && cpy._groupsSize > 0) {
        _groups = new CommandGroup*[cpy._groupsSize];
        for(int i = 0; i < cpy._groupsSize; i++) {
            _groups[i] = cpy._groups[i];
        }
        _groupsSize = cpy._groupsSize;
    } else {
        _groups = NULL;
        _groupsSize = 0;
    }
    _groupsIterator = -1;
    setGroupsEnd();
}

CommandGroup::~CommandGroup() {
    if(_commands != NULL && _commandsSize > 0) {
        delete[] _commands;
        _commands = NULL;
        _commandsSize = 0;
    }
    if(_groups != NULL && _groupsSize > 0) {
        delete[] _groups;
        _groups = NULL;
        _groupsSize = 0;
    }
}

std::string CommandGroup::getName() {
    return _name;
}

bool CommandGroup::hasCommand(std::string name) {
    bool found = false;
    if(_commands != NULL && _commandsSize >  0) {
        for(int i = 0; i < _commandsSize; i++) {
            if(_commands[i]->getName() == name) {
                found = true;
                break;
            }
        }
    }
    return found;
}

bool CommandGroup::recursiveHasCommand(std::string name) {
    bool found = false;
    if(_commands != NULL && _commandsSize >  0) {
        for(int i = 0; i < _commandsSize; i++) {
            if(_commands[i]->getName() == name) {
                found = true;
                break;
            }
        }
    }
    if(!found) {
        if(_groups != NULL && _groupsSize > 0) {
            for(int i = 0; i < _groupsSize; i++) {
                if(_groups[i]->recursiveHasCommand(name)) {
                    found = true;
                    break;
                }
            }
        }
    }
    return found;
}

bool CommandGroup::hasGroup(std::string name) {
    bool found = false;
    if(_groups != NULL && _groupsSize >  0) {
        for(int i = 0; i < _groupsSize; i++) {
            if(_groups[i]->getName() == name) {
                found = true;
                break;
            }
        }
    }
    return found;
}

bool CommandGroup::recursiveHasGroup(std::string name) {
    bool found = false;
    if(_groups != NULL && _groupsSize >  0) {
        for(int i = 0; i < _groupsSize; i++) {
            if(_groups[i]->getName() == name) {
                found = true;
                break;
            }
        }
    }
    if(!found) {
        if(_groups != NULL && _groupsSize > 0) {
            for(int i = 0; i < _groupsSize; i++) {
                if(_groups[i]->recursiveHasGroup(name)) {
                    found = true;
                    break;
                }
            }
        }
    }
    return found;
}

void CommandGroup::addPermission(Permission* p) {
    if(!p->valid()) {
        throw RCF::Common::InvalidObjectException("Permission is invalid and cannot be added!");
    }
    if(_commands != NULL && _commandsSize >  0) {
        for(int i = 0; i < _commandsSize; i++) {
            _commands[i]->addPermission(p);
        }
    }
}

void CommandGroup::recursiveAddPermission(Permission* p) {
    if(!p->valid()) {
        throw RCF::Common::InvalidObjectException("Permission is invalid and cannot be added!");
    }
    if(_commands != NULL && _commandsSize >  0) {
        for(int i = 0; i < _commandsSize; i++) {
            _commands[i]->addPermission(p);
        }
    }
    if(_groups != NULL && _groupsSize >  0) {
        for(int i = 0; i < _groupsSize; i++) {
            _groups[i]->recursiveAddPermission(p);
        }
    }
}

Command* CommandGroup::getNextCommand() {
    if(_commandsIterator+1 < _commandsSize) {
        _commandsIterator++;
        setCommandsEnd();
        return _commands[_commandsIterator];
    }
    throw RCF::Common::AtEndException("Commands iterator reached its end!");
}

Command* CommandGroup::getCommandByName(std::string name) {
    for(int i = 0; i < _commandsSize; i++) {
        if(_commands[i]->getName() == name) {
            return _commands[i];
        }
    }
    throw RCF::Common::NotFoundException(name, "Command could not be found in the command group!");
}

bool CommandGroup::commandsAtEnd() {
    setCommandsEnd();
    return _commandsEnd;
}

void CommandGroup::addCommand(Command* c) {
    if(!c->valid()) {
        throw RCF::Common::InvalidObjectException("Command is invalid and cannot be added!");
    }
    int newCommandsSize = _commandsSize + 1;
    Command** newCommands = new Command*[newCommandsSize];
    if(_commands != NULL && _commandsSize > 0) {
        for(int i = 0; i < _commandsSize; i++) {
            newCommands[i] = _commands[i];
        }
        delete[] _commands;
    }
    newCommands[newCommandsSize-1] = c;
    _commands = newCommands;
    _commandsSize = newCommandsSize;
    setCommandsEnd();
}

void CommandGroup::resetCommandsIterator() {
    _commandsIterator = -1;
    setCommandsEnd();
}

CommandGroup* CommandGroup::getNextGroup() {
    if(_groupsIterator+1 < _groupsSize) {
        _groupsIterator++;
        setGroupsEnd();
        return _groups[_groupsIterator];
    }
    throw RCF::Common::AtEndException("Groups iterator reached its end!");
}

CommandGroup* CommandGroup::getGroupByName(std::string name) {
    for(int i = 0; i < _groupsSize; i++) {
        if(_groups[i]->getName() == name) {
            return _groups[i];
        }
    }
    throw RCF::Common::NotFoundException(name, "Command group could not be found in the command group!");
}

bool CommandGroup::groupsAtEnd() {
    setGroupsEnd();
    return _groupsEnd;
}

void CommandGroup::addGroup(CommandGroup* cg) {
    if(!cg->valid()) {
        throw RCF::Common::InvalidObjectException("Command group is invalid and cannot be added!");
    }
    int newGroupsSize = _groupsSize + 1;
    CommandGroup** newGroups = new CommandGroup*[newGroupsSize];
    if(_groups != NULL && _groupsSize > 0) {
        for(int i = 0; i < _groupsSize; i++) {
            newGroups[i] = _groups[i];
        }
        delete[] _groups;
    }
    newGroups[newGroupsSize-1] = cg;
    _groups = newGroups;
    _groupsSize = newGroupsSize;
    setGroupsEnd();
}

void CommandGroup::resetGroupsIterator() {
    _groupsIterator = -1;
    setGroupsEnd();
}

void CommandGroup::save() {
    if(valid()) {
        RCF::Common::HelperFunctions hf;
        boost::filesystem::path confdir = hf.getHomeDirectory();
        confdir += "/.rcfserver";
        boost::filesystem::path cmddir = confdir;
        cmddir += "/commands";
        boost::filesystem::path filepath = cmddir;
        filepath += "/";
        filepath += _name;
        filepath += ".rcfcgp";
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
        out << "[name]" << std::endl << _name << std::endl;
        out << "[commands]" << std::endl;
        for(int i = 0; i < _commandsSize; i++) {
            out << _commands[i]->getName() << std::endl;
        }
        out << "[commandsend]" << std::endl  << "[groups]" << std::endl;
        for(int i = 0; i < _groupsSize; i++) {
            out << _groups[i]->getName()  << std::endl;
        }
        out << "[groupsend]" << std::endl;
        out.close();
    } else {
        throw RCF::Common::InvalidObjectException("Command group is invalid and cannot be saved!");
    }
}

CommandGroup* CommandGroup::load(std::string name) {
    if(_loadedGroups != NULL && _loadedGroupsSize > 0) {
        for(int i = 0; i < _loadedGroupsSize; i++) {
            if(_loadedGroups[i]->getName() == name) {
                return _loadedGroups[i];
            }
        }
    }
    CommandGroup* cg = new CommandGroup;
    RCF::Common::HelperFunctions hf;
    boost::filesystem::path confdir = hf.getHomeDirectory();
    confdir += "/.rcfserver";
    boost::filesystem::path cmddir = confdir;
    cmddir += "/commands";
    boost::filesystem::path filepath = cmddir;
    filepath += "/";
    filepath += name;
    filepath += ".rcfcgp";
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
                cg->_name = line;
                ps = "toplevel";
            } else if(ps == "commands") {
                if(line[0] == '[' && line[line.length()-1] == ']') {
                    if(line.substr(1, line.length()-2) == "commandsend") {
                        ps = "toplevel";
                    } else {
                        throw RCF::Common::ParserException(filepath, lineno, "Expected [commandsend] or username, got " + line + "!");
                    }
                } else {
                    Command* c = Command::load(line);
                    cg->addCommand(c);
                }
            } else if(ps == "groups") {
                if(line[0] == '[' && line[line.length()-1] == ']') {
                    if(line.substr(1, line.length()-2) == "groupsend") {
                        ps = "toplevel";
                    } else {
                        throw RCF::Common::ParserException(filepath, lineno, "Expected [groupsend] or username, got " + line + "!");
                    }
                } else {
                    CommandGroup* bcg = CommandGroup::load(line);
                    cg->addGroup(bcg);
                }
            } else {
                throw RCF::Common::ParserException(filepath, lineno, "Unknown parameter: " + line + "!");
            }
        }
    }
    in.close();
    int newLoadedGroupsSize = _loadedGroupsSize + 1;
    CommandGroup** newLoadedGroups = new CommandGroup*[newLoadedGroupsSize];
    if(_loadedGroups != NULL && _loadedGroupsSize > 0) {
        for(int i = 0; i < _loadedGroupsSize; i++) {
            newLoadedGroups[i] = _loadedGroups[i];
        }
        delete[] _loadedGroups;
    }
    newLoadedGroups[newLoadedGroupsSize-1] = cg;
    _loadedGroups = newLoadedGroups;
    _loadedGroupsSize = newLoadedGroupsSize;
    return cg;
}

bool CommandGroup::valid() {
    if(_name != "") {
        return true;
    }
    return false;
}

void CommandGroup::free() {
    if(_loadedGroups != NULL && _loadedGroupsSize > 0) {
        for(int i = 0; i < _loadedGroupsSize; i++) {
            delete _loadedGroups[i];
        }
        delete[] _loadedGroups;
    }
    _loadedGroups = NULL;
    _loadedGroupsSize = 0;
}
