#ifndef _CLIENT_H
#define _CLIENT_H
#include <boost/predef.h>
#if BOOST_OS_WINDOWS
    #define _WIN32_WINNT 0x0501
    #define WINVER 0x0501
#endif
#include <librcfcommon/librcfcommon.h>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
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
    /// \namespace Client
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
        public:
            Client(boost::asio::io_service& service, std::string sdName); ///< \brief A constructor from boost::asio::io_service and server definition name.
            ///< \param sdName Server definition name.
            ~Client(); ///< A destructor.
            std::string read(); ///< \brief Reads next portion of the transmission.
            ///< \return Received data without ending separator as std::string.
            void write(std::string trans); ///< \brief Writes next portion of the transmission, adding the ending separator.
            ///< \param trans Transmission to write.
        };
    }
}
#endif
