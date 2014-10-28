#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

namespace RCF {
    namespace Tests {
        class SSLTestClient {
        private:
            ssl_socket* _socket;
            enum { max_length = 1024 };
            char _data[max_length];
            boost::asio::io_service& _io_service;
            boost::asio::ssl::context _ctx;
            boost::asio::ip::tcp::resolver _rslvr;
        public:
            SSLTestClient(boost::asio::io_service& io_service, std::string hostname, std::string port): _io_service(io_service), _rslvr(_io_service), _ctx(boost::asio::ssl::context::sslv23) {
                _ctx.set_verify_mode(boost::asio::ssl::verify_peer);
                _ctx.load_verify_file("cert/testsrvcrt.pem"); // Ładujemy certyfikat serwera
                _socket = new ssl_socket(_io_service, _ctx);
                boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp> endpoint = _rslvr.resolve(boost::asio::ip::basic_resolver_query<boost::asio::ip::tcp>(hostname, port));
                _socket->next_layer().connect(*endpoint);
                _socket->handshake(boost::asio::ssl::stream_base::client);
            }
            ~SSLTestClient() {
                delete _socket;
            }
            void write_data(std::string d) {
                std::string tosend = "MSG ";
                tosend += d;
                for(int i = 0; i < tosend.length(); i++) {
                    _data[i] = tosend[i];
                }
                _data[tosend.length()] = '\000';
                _socket->write_some(boost::asio::buffer(_data, max_length));
            }
        };
    }
}

int main() {
    std::cout << "SSL Test Client - start..." << std::endl;
    boost::asio::io_service srvc;
    RCF::Tests::SSLTestClient cli(srvc, "127.0.0.1", "8888");
    std::cout << "Enter data to send: ";
    std::string data;
    std::cin >> data;
    cli.write_data(data);
    std::cout << "All done!" << std::endl;
    return EXIT_SUCCESS;
}
