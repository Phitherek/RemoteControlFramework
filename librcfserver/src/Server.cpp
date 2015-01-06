#include "Server.h"
#include <librcfcommon/librcfcommon.h>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <sstream>
using namespace RCF::Server;

Server::Server(boost::asio::io_service& service, unsigned int port, void (*mainfn)(Server*, ssl_socket*)):  _io_service(service), _acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), _ctx(boost::asio::ssl::context::sslv23) {
    _ctx.set_options(boost::asio::ssl::context::default_workarounds | boost::asio::ssl::context::single_dh_use);
    RCF::Common::HelperFunctions hf;
    boost::filesystem::path ssldir = hf.getHomeDirectory();
    ssldir += "/.rcfserver/ssl";
    boost::filesystem::path certfile = ssldir;
    certfile += "/cert/rcfservercert.pem";
    boost::filesystem::path dhfile = ssldir;
    dhfile += "/dh/rcfserverdh.pem";
    boost::filesystem::path keyfile = ssldir;
    keyfile += "/key/rcfserverkey.pem";
    if(!boost::filesystem::exists(certfile)) {
        throw RCF::Common::FilesystemException(certfile, "Does not exist!");
    }
    if(!boost::filesystem::exists(dhfile)) {
        throw RCF::Common::FilesystemException(dhfile, "Does not exist!");
    }
    if(!boost::filesystem::exists(keyfile)) {
        throw RCF::Common::FilesystemException(keyfile, "Does not exist!");
    }
    _ctx.use_certificate_chain_file(certfile.string());
    _ctx.use_private_key_file(keyfile.string(), boost::asio::ssl::context::pem);
    _ctx.use_tmp_dh_file(dhfile.string());
    boost::filesystem::path permdir = hf.getHomeDirectory();
    permdir += "/.rcfserver/perms";
    boost::filesystem::path cmddir = hf.getHomeDirectory();
    cmddir += "/.rcfserver/commands";
    if(!boost::filesystem::exists(permdir)) {
        throw RCF::Common::FilesystemException(permdir, "Does not exist!");
    }
    if(!boost::filesystem::exists(cmddir)) {
        throw RCF::Common::FilesystemException(cmddir, "Does not exist!");
    }
    boost::filesystem::directory_iterator permiter(permdir);
    boost::filesystem::directory_iterator cmditer(cmddir);
    boost::filesystem::directory_iterator enditer;
    while(permiter != enditer) {
        bool load = true;
        int permsSize = _perms.size();
        for(int i = 0; i < permsSize; i++) {
            if(_perms[i]->getName() == (*permiter).path().stem().string()) {
                load = false;
                break;
            }
        }
        if(load) {
            _perms.push_back(RCF::Server::Group::load((*permiter).path().stem().string()));
        }
        permiter++;
    }
    while(cmditer != enditer) {
        bool load = true;
        if((*cmditer).path().extension().string() == ".rcfcmd") {
            int commandsSize = _commands.size();
            for(int i = 0; i < commandsSize; i++) {
                if(_commands[i]->getName() == (*cmditer).path().stem().string()) {
                    load = false;
                    break;
                }
            }
            if(load) {
                int groupsSize = _groups.size();
                for(int i = 0; i < groupsSize; i++) {
                    if(_groups[i]->recursiveHasCommand((*cmditer).path().stem().string())) {
                        load = false;
                        break;
                    }
                }
            }
            if(load) {
                _commands.push_back(RCF::Server::Command::load((*cmditer).path().stem().string()));
            }
        } else if((*cmditer).path().extension().string() == ".rcfcgp") {
            int groupsSize = _groups.size();
            for(int i = 0; i < groupsSize; i++) {
                if(_groups[i]->getName() == (*cmditer).path().stem().string() || _groups[i]->recursiveHasGroup((*cmditer).path().stem().string())) {
                    load = false;
                    break;
                }
            }
            if(load) {
                _groups.push_back(RCF::Server::CommandGroup::load((*cmditer).path().stem().string()));
            }
        }
        cmditer++;
    }
    _mainfn = mainfn;
}

Server::~Server() {
    CommandGroup::free();
    Command::free();
    Permission::free();
}

void Server::handleClient(ssl_socket* sock) {
    sock->handshake(boost::asio::ssl::stream_base::server);
    boost::this_thread::at_thread_exit(boost::bind(&Server::handleThreadExit, this, sock));
    _mainfn(this, sock);
}

void Server::handleThreadExit(ssl_socket* sock) {
    sock->lowest_layer().close();
    delete sock;
}

void Server::handleSublist(ssl_socket* sock, User* logged_user, CommandGroup* grp) {
    std::string name = "[";
    name += grp->getName();
    name += "]";
    write(sock, name);
    while(!grp->commandsAtEnd()) {
        try {
            Command* cmd = grp->getNextCommand();
            if(cmd->canExecute(logged_user->getName())) {
                write(sock, cmd->getName());
            }
        } catch(RCF::Common::AtEndException& e) {
            break;
        }
    }
    grp->resetCommandsIterator();
    while(!grp->groupsAtEnd()) {
        try {
            CommandGroup* subgrp = grp->getNextGroup();
            handleSublist(sock, logged_user, subgrp);
        } catch(RCF::Common::AtEndException& e) {
            break;
        }
    }
    grp->resetGroupsIterator();
    write(sock, "[end]");
}

void Server::start() {
    while(true) {
        ssl_socket* sock = new ssl_socket(_io_service, _ctx);
        _acceptor.accept(sock->lowest_layer());
        boost::thread*  handlingThread = new boost::thread(&Server::handleClient, this, sock);
        _threads.add_thread(handlingThread);
    }
}

void Server::handshake(ssl_socket* sock) {
    std::string check = read(sock);
    if(check == "RCFINIT") {
        write(sock, "RCFINIT");
    } else {
        throw RCF::Common::ProtocolException("Client did not initialize RCF Protocol properly!");
    }
}

User* Server::authorize(ssl_socket* sock) {
    std::string cmd = read(sock);
    if(cmd.substr(0, 4) == "USER") {
        std::string un = cmd.substr(5, std::string::npos);
        int _permsSize = _perms.size();
        for(int i = 0; i < _permsSize; i++) {
            if(_perms[i]->getPermissionType() == "user" && _perms[i]->getName() == un) {
                write(sock, "PASS");
                cmd = read(sock);
                if(cmd.substr(0, 4) == "PASS") {
                    std::string pass = cmd.substr(5, std::string::npos);
                    if(dynamic_cast<User*>(_perms[i])->checkPassword(pass)) {
                        write(sock, "AUTHORIZED");
                        return dynamic_cast<User*>(_perms[i]);
                    } else {
                        throw RCF::Common::NotFoundException(un, "User does not exist or bad password!");
                    }
                } else {
                    throw RCF::Common::ProtocolException("Expected response to password query!");
                }
            }
        }
        throw RCF::Common::NotFoundException(un, "User does not exist or bad password!");
    } else {
        throw RCF::Common::ProtocolException("Expected user authorization start!");
    }
}

void Server::handlePostauth(ssl_socket* sock, User* logged_user) {
    std::string cmd = read(sock);
    if(cmd == "LIST") {
        handleList(sock, logged_user);
    } else if(cmd.substr(0, 4) == "EXEC") {
        handleExecute(sock, cmd.substr(5, std::string::npos), logged_user);
    } else if(cmd == "CLOSE") {
        close(sock);
    } else {
        throw RCF::Common::ProtocolException("Unknown protocol command received!");
    }
}

void Server::handleList(ssl_socket* sock, User* logged_user) {
    write(sock, "BEGINLIST");
    int commandsSize = _commands.size();
    for(int i = 0; i < commandsSize; i++) {
        if(_commands[i]->canExecute(logged_user->getName())) {
            write(sock, _commands[i]->getName());
        }
    }
    int groupsSize = _groups.size();
    for(int i = 0; i < groupsSize; i++) {
        handleSublist(sock, logged_user, _groups[i]);
    }
    write(sock, "ENDLIST");
}

void Server::handleExecute(ssl_socket* sock, std::string query, User* logged_user) {
    std::vector<std::string> splittedQuery;
    int queryLength = query.length();
    std::string part = "";
    for(int i = 0; i < queryLength; i++) {
        if(query[i] == ':') {
            splittedQuery.push_back(part);
            part = "";
        } else {
            part += query[i];
        }
    }
    int splittedQuerySize = splittedQuery.size();
    CommandGroup* curgrp = NULL;
    for(int i = 0; i < splittedQuerySize; i++) {
        if(i == splittedQuerySize-1) {
            if(curgrp == NULL) {
                int commandsSize = _commands.size();
                for(int j = 0; j < commandsSize; j++) {
                    if(_commands[j]->getName() == splittedQuery[i]) {
                        if(_commands[j]->canExecute(logged_user->getName())) {
                            if(_commands[j]->numParams() > 0) {
                                std::vector<std::string> params;
                                for(int k = 0; k < _commands[j]->numParams(); k++) {
                                    write(sock, "PARAM");
                                    std::string param = read(sock);
                                    params.push_back(param);
                                }
                                write(sock, "EXECBEGIN");
                                int code;
                                std::string out;
                                std::string err;
                                code = _commands[j]->execute(&out, &err, params);
                                std::stringstream msgss;
                                msgss.str("");
                                msgss << "EXECEND " << code;
                                write(sock, msgss.str());
                                write(sock, "OUTBEGIN");
                                write(sock, out);
                                write(sock, "OUTEND");
                                write(sock, "ERRBEGIN");
                                write(sock, err);
                                write(sock, "ERREND");
                                return;
                            } else {
                                write(sock, "EXECBEGIN");
                                int code;
                                std::string out;
                                std::string err;
                                code = _commands[j]->execute(&out, &err);
                                std::stringstream msgss;
                                msgss.str("");
                                msgss << "EXECEND " << code;
                                write(sock, msgss.str());
                                write(sock, "OUTBEGIN");
                                write(sock, out);
                                write(sock, "OUTEND");
                                write(sock, "ERRBEGIN");
                                write(sock, err);
                                write(sock, "ERREND");
                                return;
                            }
                        } else {
                            throw RCF::Common::NotFoundException(query, "Query could not be resolved!");
                        }
                        break;
                    }
                }
            } else {
                if(curgrp->hasCommand(splittedQuery[i])) {
                    Command* cmd = curgrp->getCommandByName(splittedQuery[i]);
                    if(cmd->canExecute(logged_user->getName())) {
                        if(cmd->numParams() > 0) {
                                std::vector<std::string> params;
                                for(int k = 0; k < cmd->numParams(); k++) {
                                    write(sock, "PARAM");
                                    std::string param = read(sock);
                                    params.push_back(param);
                                }
                                write(sock, "EXECBEGIN");
                                int code;
                                std::string out;
                                std::string err;
                                code = cmd->execute(&out, &err, params);
                                std::stringstream msgss;
                                msgss.str("");
                                msgss << "EXECEND " << code;
                                write(sock, msgss.str());
                                write(sock, "OUTBEGIN");
                                write(sock, out);
                                write(sock, "OUTEND");
                                write(sock, "ERRBEGIN");
                                write(sock, err);
                                write(sock, "ERREND");
                                return;
                            } else {
                                write(sock, "EXECBEGIN");
                                int code;
                                std::string out;
                                std::string err;
                                code = cmd->execute(&out, &err);
                                std::stringstream msgss;
                                msgss.str("");
                                msgss << "EXECEND " << code;
                                write(sock, msgss.str());
                                write(sock, "OUTBEGIN");
                                write(sock, out);
                                write(sock, "OUTEND");
                                write(sock, "ERRBEGIN");
                                write(sock, err);
                                write(sock, "ERREND");
                                return;
                            }
                    }
                } else {
                    throw RCF::Common::NotFoundException(query, "Query could not be resolved!");
                }
            }
        } else {
            if(curgrp == NULL) {
                int groupsSize = _groups.size();
                for(int j = 0; j < groupsSize; j++) {
                    if(_groups[j]->getName() == splittedQuery[i]) {
                        curgrp = _groups[j];
                        break;
                    }
                }
                if(curgrp == NULL) {
                    throw RCF::Common::NotFoundException(query, "Query could not be resolved!");
                }
            } else {
                if(curgrp->hasGroup(splittedQuery[i])) {
                    curgrp = curgrp->getGroupByName(splittedQuery[i]);
                } else {
                    throw RCF::Common::NotFoundException(query, "Query could not be resolved!");
                }
            }
        }
    }
    throw RCF::Common::NotFoundException(query, "Query could not be resolved!");
}

void Server::close(ssl_socket* sock) {
    write(sock, "CLOSE");
}

void Server::handleException(ssl_socket* sock, std::exception& e) {
    std::string msg = "ERROR ";
    msg += e.what();
    write(sock, msg);
}

std::string Server::read(ssl_socket* sock) {
    char data[max_length];
    std::string out = "";
    while(true) {
        for(int i = 0; i < max_length; i++) {
            data[i] = '\0';
        }
        int length = sock->read_some(boost::asio::buffer(data, max_length));
        if(data[length-1] == 3) {
            for(int i = 0; i < length-1; i++) {
                out += data[i];
            }
            break;
        } else {
            for(int i = 0; i < length; i++) {
                out += data[i];
            }
        }
    }
    return out;
}

void Server::write(ssl_socket* sock, std::string trans) {
    char data[max_length];
    int pos = 0;
    int transLength = trans.length();
    bool addEnd = false;
    int addEndPos = 0;
    bool additionalEndSend = false;
    while(pos < transLength) {
        for(int i = 0; i < max_length; i++) {
            data[i] = '\0';
        }
        if(transLength-pos > max_length-1) {
            for(int i = 0; i < max_length; i++) {
                data[i] = trans[pos+i];
            }
            pos += max_length;
        } else if(transLength-pos == max_length-1) {
            for(int i = 0; i < max_length; i++) {
                data[i] = trans[pos+i];
            }
            pos += max_length;
            additionalEndSend = true;
        } else {
            int i;
            for(i = 0; i < transLength-pos; i++) {
                data[i] = trans[pos+i];
            }
            pos = transLength;
            addEndPos = i;
            addEnd = true;
        }
        if(addEnd) {
            data[addEndPos] = 3;
        }
        sock->write_some(boost::asio::buffer(data, max_length));
    }
    if(additionalEndSend) {
        for(int i = 0; i < max_length; i++) {
            data[i] = '\0';
        }
        data[0] = 3;
        sock->write_some(boost::asio::buffer(data, max_length));
    }
}
