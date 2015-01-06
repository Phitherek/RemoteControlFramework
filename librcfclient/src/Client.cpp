#include "Client.h"
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
}

Client::~Client() {
    delete _sock;
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
