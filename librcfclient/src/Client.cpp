#include "Client.h"
#include <cstdlib>
#include <sstream>
#include <iostream>
using namespace RCF::Client;

Client::Client(boost::asio::io_service& service, std::string sdName): _io_service(service), _ctx(boost::asio::ssl::context::sslv23), _rslvr(service)  {
    _srvdef = ServerDefinition::load(sdName);
    _ctx.set_verify_mode(boost::asio::ssl::verify_peer);
    _ctx.load_verify_file(_srvdef->getCertFile().string()); // Ładujemy certyfikat serwera
    _sock = new ssl_socket(_io_service, _ctx);
    boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp> end;
    boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp> endpoint = _rslvr.resolve(boost::asio::ip::basic_resolver_query<boost::asio::ip::tcp>(_srvdef->getHostname(), "8910"));
    while(endpoint != end) {
        try {
            _sock->next_layer().connect(*endpoint);
            break;
        } catch(boost::system::system_error& e) {
            endpoint++;
            if(endpoint == end) {
                throw e;
            }
        }
    }
    _sock->handshake(boost::asio::ssl::stream_base::client);
    _logged_username = "";
}

Client::~Client() {
}

ServerDefinition* Client::getServerDefinition() {
    return _srvdef;
}

void Client::handshake() {
    write("RCFINIT");
    std::string resp = read();
    if(resp != "RCFINIT") {
        throw RCF::Common::ProtocolException("Server did not properly respond to RCF Protocol handshake!");
    }
}

void Client::authorize(std::string username, std::string password) {
    std::string msg = "";
    std::string resp = "";
    msg += "USER ";
    msg += username;
    write(msg);
    resp = read();
    if(resp == "PASS") {
        msg = "PASS ";
        msg += password;
        write(msg);
        resp = read();
        if(resp != "AUTHORIZED" && resp.substr(0, 5) == "ERROR") {
            std::string err = resp.substr(6, std::string::npos);
            throw RCF::Common::ProtocolException(err);
        }
        _logged_username = username;
    } else if(resp.substr(0, 5) == "ERROR") {
        std::string err = resp.substr(6, std::string::npos);
        throw RCF::Common::ProtocolException(err);
    }
}

std::string Client::list() {
    std::string outlist = "";
    std::stringstream outlistss;
    outlistss.str("");
    std::string msg = "";
    std::string resp = "";
    msg = "LIST";
    write(msg);
    resp = read();
    if(resp == "BEGINLIST") {
        while(resp != "ENDLIST") {
            resp = read();
            if(resp != "ENDLIST") {
                outlistss << resp << std::endl;
            }
        }
    } else {
        throw RCF::Common::ProtocolException("Unexpected protocol message!");
    }
    outlist = outlistss.str();
    return outlist;
}

int Client::execute(std::string query, std::string* stdout_target, std::string* stderr_target, std::string (*paramProvider)()) {
    std::string msg = "";
    std::string resp = "";
    int code;
    std::string ps = "toplevel";
    msg += "EXEC ";
    msg += query;
    write(msg);
    do {
        resp = read();
        if(ps == "toplevel") {
            if(resp == "PARAM") {
                std::string param;
                param = paramProvider();
                write(param);
            } else if(resp == "EXECBEGIN") {
                ps = "inexec";
            } else if(resp == "OUTBEGIN") {
                ps = "out";
            } else if(resp == "ERRBEGIN") {
                ps = "err";
            } else if(resp.substr(0, 5) == "ERROR") {
                std::string err = msg.substr(6, std::string::npos);
                throw RCF::Common::ProtocolException(err);
            } else if(resp.substr(0, 7) == "NCERROR") {
                std::string err = msg.substr(8, std::string::npos);
                throw RCF::Common::NotFoundException("From server", err);
            } else {
                throw RCF::Common::ProtocolException("Unexpected protocol message!");
            }
        } else if(ps == "inexec") {
            if(resp.substr(0, 7) == "EXECEND") {
                std::string scode = resp.substr(8, std::string::npos);
                code = atoi(scode.c_str());
                ps = "toplevel";
            } else {
                if(resp.substr(0, 5) == "ERROR") {
                    std::string err = msg.substr(6, std::string::npos);
                    throw RCF::Common::ProtocolException(err);
                } else {
                    throw RCF::Common::ProtocolException("Unexpected protocol message!");
                }
            }
        } else if(ps == "out") {
            if(resp == "OUTEND") {
                ps = "toplevel";
            } else {
                *stdout_target += resp;
            }
        } else if(ps == "err") {
            if(resp == "ERREND") {
                ps = "toplevel";
            } else {
                *stderr_target += resp;
            }
        }
    } while(resp != "ERREND");
    return code;
}

int Client::execute(std::string query, std::string* stdout_target, std::string* stderr_target, std::string (*paramProvider)(Client*)) {
    std::string msg = "";
    std::string resp = "";
    int code;
    std::string ps = "toplevel";
    msg += "EXEC ";
    msg += query;
    write(msg);
    do {
        resp = read();
        if(ps == "toplevel") {
            if(resp == "PARAM") {
                std::string param;
                param = paramProvider(this);
                write(param);
            } else if(resp == "EXECBEGIN") {
                ps = "inexec";
            } else if(resp == "OUTBEGIN") {
                ps = "out";
            } else if(resp == "ERRBEGIN") {
                ps = "err";
            } else if(resp.substr(0, 5) == "ERROR") {
                std::string err = msg.substr(6, std::string::npos);
                throw RCF::Common::ProtocolException(err);
            } else if(resp.substr(0, 7) == "NCERROR") {
                std::string err = msg.substr(8, std::string::npos);
                throw RCF::Common::NotFoundException("From server", err);
            } else {
                throw RCF::Common::ProtocolException("Unexpected protocol message!");
            }
        } else if(ps == "inexec") {
            if(resp.substr(0, 7) == "EXECEND") {
                std::string scode = resp.substr(8, std::string::npos);
                code = atoi(scode.c_str());
                ps = "toplevel";
            } else {
                if(resp.substr(0, 5) == "ERROR") {
                    std::string err = msg.substr(6, std::string::npos);
                    throw RCF::Common::ProtocolException(err);
                } else {
                    throw RCF::Common::ProtocolException("Unexpected protocol message!");
                }
            }
        } else if(ps == "out") {
            if(resp == "OUTEND") {
                ps = "toplevel";
            } else {
                *stdout_target += resp;
            }
        } else if(ps == "err") {
            if(resp == "ERREND") {
                ps = "toplevel";
            } else {
                *stderr_target += resp;
            }
        }
    } while(resp != "ERREND");
    return code;
}

void Client::close() {
    write("CLOSE");
    std::string resp = read();
    if(resp != "CLOSE") {
        throw RCF::Common::ProtocolException("Unexpected protocol message!");
    }
}

std::string Client::read() {
    std::string out = "";
    bool finish = false;
    while(!finish) {
        for(int i = 0; i < max_length; i++) {
            _data[i] = '\0';
        }
        int length = _sock->read_some(boost::asio::buffer(_data, max_length));
        for(int i = 0; i < length; i++) {
            if(_data[i] == 3) {
                finish = true;
                break;
            } else {
                out += _data[i];
            }
        }
    }
    return out;
}

void Client::write(std::string trans) {
    int pos = 0;
    int transLength = trans.length();
    bool addEnd = false;
    int addEndPos = 0;
    bool additionalEndSend = false;
    while(pos < transLength) {
        for(int i = 0; i < max_length; i++) {
            _data[i] = '\0';
        }
        if(transLength-pos > max_length-1) {
            for(int i = 0; i < max_length; i++) {
                _data[i] = trans[pos+i];
            }
            pos += max_length;
        } else if(transLength-pos == max_length-1) {
            for(int i = 0; i < max_length; i++) {
                _data[i] = trans[pos+i];
            }
            pos += max_length;
            additionalEndSend = true;
        } else {
            int i;
            for(i = 0; i < transLength-pos; i++) {
                _data[i] = trans[pos+i];
            }
            pos = transLength;
            addEndPos = i;
            addEnd = true;
        }
        if(addEnd) {
            _data[addEndPos] = 3;
        }
        _sock->write_some(boost::asio::buffer(_data, max_length));
    }
    if(additionalEndSend) {
        for(int i = 0; i < max_length; i++) {
            _data[i] = '\0';
        }
        _data[0] = 3;
        _sock->write_some(boost::asio::buffer(_data, max_length));
    }
}

std::string Client::getLoggedUsername() {
    return _logged_username;
}
