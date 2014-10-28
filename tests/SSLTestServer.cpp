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
        class SSLTestServer {
        private:
            ssl_socket* _socket; // Nakładka SSL na zwykły TCP socket
            enum { max_length = 1024 };
            char _data[max_length];
            boost::asio::io_service& _io_service; // Obiekt wymagany przez ASIO we wszystkich funkcjach
            boost::asio::ip::tcp::acceptor _acceptor; // Akceptor dla serwera
            boost::asio::ssl::context _ctx; // Kontekst SSL
        public:
            SSLTestServer(boost::asio::io_service& io_service, unsigned short port): _io_service(io_service), _acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), _ctx(boost::asio::ssl::context::sslv23) { // Inicjujemy akceptor dla IPv4 i kontekst dla SSL v. 2 i 3 oraz bufor
                _ctx.set_options(boost::asio::ssl::context::default_workarounds | boost::asio::ssl::context::single_dh_use); // Domyślne obejścia różnych bugów, zawsze nowy klucz dla parametrów tmp_dh.
                _ctx.use_certificate_chain_file("cert/testsrvcrt.pem"); // Certyfikat serwera
                _ctx.use_private_key_file("key/testsrvkey.pem", boost::asio::ssl::context::pem); // Klucz prywatny serwera
                _ctx.use_tmp_dh_file("dh/testsrvdh.pem"); // Parametry DH serwera
                _socket = new ssl_socket(_io_service, _ctx); // Nowy socket dla io_service i kontekstu SSL
                _acceptor.accept(_socket->lowest_layer()); // Uruchamiamy akceptor
                _socket->handshake(boost::asio::ssl::stream_base::server); // SSL handshake z klientem
            }
            ~SSLTestServer() {
                _acceptor.close();
                delete _socket;
            }
            std::string read_data() {
                int length = _socket->read_some(boost::asio::buffer(_data, max_length));
                std::string strdata = "";
                for(int i = 0; i < length && _data[i] != '\000'; i++) {
                    strdata += _data[i];
                }
                if(strdata.find("MSG") != std::string::npos) {
                    return strdata.substr(strdata.find("MSG")+4);
                } else {
                    std::cerr << "Protocol error!" << std::endl;
                    return "";
                }
            }
        };
    }
}

int main() {
    std::cout << "SSL Test Server - start..." << std::endl;
    boost::asio::io_service srvc;
    RCF::Tests::SSLTestServer srv(srvc, 8888);
    std::string rcv = srv.read_data();
    if(rcv == "") {
        std::cerr << "Error occured!" << std::endl;
    } else {
        std::cout << "Received message: " << rcv << std::endl;
    }
    std::cout << "All done!" << std::endl;
    return EXIT_SUCCESS;
}
