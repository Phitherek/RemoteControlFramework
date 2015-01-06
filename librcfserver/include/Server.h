#ifndef _SERVER_H
#define _SERVER_H
#include "Permission.h"
#include "User.h"
#include "Group.h"
#include "ExecuteImpl.h"
#include "Command.h"
#include "CommandGroup.h"
#include <librcfcommon/librcfcommon.h>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/thread.hpp>
#include <exception>
#include <typeinfo>
#include <vector>
/// \file Server.h
/// \brief A class that can be used to create RCF server and contains useful RCF protocol methods.
/// \author Phitherek_
/// \date 2015
/// \version 0.1

/// This is a name simplification for Boost::Asio' s SSL socket stream type.
typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

/// \namespace RCF
/// \brief A global namespace for RemoteControlFramework
namespace RCF {
    /// \namespace Server
    /// \brief A namespace for RCF server
    namespace Server {
        /// \class Server
        /// \brief A class that can be used to create RCF server and contains useful RCF protocol methods.
        class Server {
        private:
            boost::thread_group _threads;
            enum { max_length = 1024 };
            boost::asio::io_service& _io_service; // Obiekt wymagany przez ASIO we wszystkich funkcjach
            boost::asio::ip::tcp::acceptor _acceptor; // Akceptor dla serwera
            boost::asio::ssl::context _ctx; // Kontekst SSL
            std::vector<Permission*> _perms;
            std::vector<Command*> _commands;
            std::vector<CommandGroup*> _groups;
            void (*_mainfn)(Server*, ssl_socket*); // Ciało serwera. Przekazanie obiektu RCF::Server::Server powoduje dostępność metod protokołu RCF w funkcji.
            void handleClient(ssl_socket* sock);
            void handleThreadExit(ssl_socket* sock);
            void handleSublist(ssl_socket* sock, User* logged_user, CommandGroup* grp);
        public:
            Server(boost::asio::io_service& service, unsigned int port, void (*mainfn)(Server*, ssl_socket*)); ///< \brief A constructor from boost::asio::io_service, port and main function.
            ///< \param _service A reference to boost::asio::io_service, that has to be constructed before server construction and is used for boost::asio communication (refer to boost::asio documentation for more information).
            ///< \param port A port the server is to be started at.
            ///< \param mainfn A pointer to the function, that returns void and takes two parameters - a pointer to RCF::Server::Server and a pointer to boost::asio::ssl::stream<boost::asio::ip::tcp::socket>. Inside the function define the server functionality using provided with this class RCF protocol methods.
            ~Server(); ///< A destructor.
            void start(); ///< Starts the server.
            void handshake(ssl_socket* sock); ///< \brief Performs the handshake with the client.
            ///< \param sock A client communication socket.
            User* authorize(ssl_socket* sock); ///< \brief Authorizes the user.
            ///< \param sock A client communication socket.
            ///< \return A pointer to authorized user.
            void handlePostauth(ssl_socket* sock, User* logged_user); ///< \brief Handles the post-authorization process.
            ///< \param sock A client communication socket.
            ///< \param logged_user A pointer to logged in user.
            void handleList(ssl_socket* sock, User* logged_user); ///< \brief Handles the command list query.
            ///< \param sock A client communication socket.
            ///< \param logged_user A pointer to logged in user.
            void handleExecute(ssl_socket* sock, std::string query, User* logged_user); ///< \brief Handles the command execute query.
            ///< \param sock A client communication socket.
            ///< \param query Command execute query.
            ///< \param logged_user A pointer to logged in user.
            void close(ssl_socket* sock); ///< \brief Closes the connection.
            ///< \param sock A client communication socket.
            void handleException(ssl_socket* sock, std::exception& e); ///< \brief Handles the exceptions.
            ///< \param sock A client communication socket.
            ///<  \param e The exception.
            std::string read(ssl_socket* sock); ///< \brief Reads next portion of the transmission.
            ///< \param sock A client communication socket.
            ///< \return Received data without ending separator as std::string.
            void write(ssl_socket* sock, std::string trans); ///< \brief Writes next portion of the transmission, adding the ending separator.
            ///< \param sock A client communication socket.
            ///< \param trans Transmission to write.
        };
    }
}
#endif
