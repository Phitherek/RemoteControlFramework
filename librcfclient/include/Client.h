#ifndef _CLIENT_H
#define _CLIENT_H
#include <boost/predef.h>
#if BOOST_OS_WINDOWS
    #define _WIN32_WINNT 0x0501
    #define WINVER 0x0501
#endif
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <librcfcommon/librcfcommon.h>
#include <string>
#include "ServerDefinition.h"
/// \file Client.h
/// \brief A class that can be used to create RCF client and contains useful RCF protocol methods.
/// \author Phitherek_
/// \date 2015
/// \version 0.1

/// This is a name simplification for Boost::Asio' s SSL socket stream type.
typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

/// \namespace RCF
/// \brief A global namespace for RemoteControlFramework
namespace RCF {
    /// \namespace RCF::Client
    /// \brief A namespace for RCF client
    namespace Client {
        /// \class Client
        /// \brief A class that can be used to create RCF client and contains useful RCF protocol methods.
        class Client {
        private:
            ServerDefinition* _srvdef;
            ssl_socket* _sock;
            enum { max_length = 1024 };
            char _data[max_length];
            boost::asio::io_service& _io_service;
            boost::asio::ssl::context _ctx;
            boost::asio::ip::tcp::resolver _rslvr;
            std::string _logged_username;
        public:
            Client(boost::asio::io_service& service, std::string sdName); ///< \brief A constructor from boost::asio::io_service and server definition name.
            ///< \param service A reference to boost::asio::io_service, that has to be constructed before server construction and is used for boost::asio communication (refer to boost::asio documentation for more information).
            ///< \param sdName Server definition name.
            ~Client(); ///< A destructor.
            ServerDefinition* getServerDefinition(); ///< \brief Returns the server definition this client has been connected to.
            ///< \return A pointer to ServerDefinition which was used to connect.
            void handshake(); ///< Performs the handshake with the server.
            void authorize(std::string username, std::string password); ///< \brief Authorize the user on the server.
            ///< \param username A username to authorize with.
            ///< \param password A password to authorize with.
            std::string list(); ///< \brief Obtains a list of commands for currently logged in user.
            ///< \return List of commands to be parsed by the client.
            int execute(std::string query, std::string* stdout_target, std::string* stderr_target, std::string (*paramProvider)()); ///< \brief Executes a command on the server.
            ///< \param query A command query - includes command name preceded by the hierarchy of groups that command belongs to, separated by : sign. Example: administration:network:mycommand, where mycommand belongs to the network group, and network group belongs to administration group which is top-level on the server.
            ///< \param stdout_target A target string where command stdout should be placed.
            ///< \param stderr_target A target string where command stderr should be placed.
            ///< \param paramProvider A function that provides additional parameters for the command (in response to PARAM protocol message). Takes no arguments, returns std::string.
            ///< \return Command exit code.
            int execute(std::string query, std::string* stdout_target, std::string* stderr_target, std::string (*paramProvider)(Client*)); ///< \brief Executes a command on the server.
            ///< \param query A command query - includes command name preceded by the hierarchy of groups that command belongs to, separated by : sign. Example: administration:network:mycommand, where mycommand belongs to the network group, and network group belongs to administration group which is top-level on the server.
            ///< \param stdout_target A target string where command stdout should be placed.
            ///< \param stderr_target A target string where command stderr should be placed.
            ///< \param paramProvider A function that provides additional parameters for the command (in response to PARAM protocol message). Takes a Client argument, returns std::string. Can be used if additional information from the client module is needed in the function.
            ///< \return Command exit code.
            void close(); ///< Closes the connection.
            std::string read(); ///< \brief Reads next portion of the transmission.
            ///< \return Received data without ending separator as std::string.
            void write(std::string trans); ///< \brief Writes next portion of the transmission, adding the ending separator.
            ///< \param trans Transmission to write.
            std::string getLoggedUsername(); ///< \brief Returns the username used to login.
            ///< \return Logged in username.
        };
    }
}
#endif
