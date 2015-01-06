#ifndef _SERVERDEFINITION_H
#define _SERVERDEFINITION_H
#include <librcfcommon/librcfcommon.h>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string>
/// \file ServerDefinition.h
/// \brief A class that represents server definition for client.
/// \author Phitherek_
/// \date 2015
/// \version 0.1

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

/// \namespace RCF
/// \brief A global namespace for RemoteControlFramework
namespace RCF {
    /// \namespace Client
    /// \brief A namespace for RCF client
    namespace Client {
        /// \class ServerDefinition
        /// \brief A class that represents server definition for client.
        class ServerDefinition {
        private:
            std::string _name;
            std::string _hostname;
            boost::filesystem::path _certfile;
            static ServerDefinition** _loadedDefinitions;
            static int _loadedDefinitionsSize;
        public:
            ServerDefinition(); ///< A plain constructor.
            ServerDefinition(std::string name, std::string hostname, std::string certfilepath); ///< \brief A constructor from name, hostname and certificate file path.
            ///< \param hostname Server hostname or IP address.
            ///< \param certfile A path to server' s SSL certificate.
            std::string getName(); ///< \brief Returns server definition name.
            ///< \return Server definition name.
            std::string getHostname(); ///< \brief Returns server hostname.
            ///< \return Server hostname.
            boost::filesystem::path getCertFile(); ///< \brief Returns a path to server' s certificate file.
            ///< \return A path to server' s certificate file, boost::filesystem::path.
            bool valid(); ///< \brief Checks if definition is valid to save or process.
            ///< \return Boolean.
            void save(); ///< Saves the server definition to file.
            static ServerDefinition* load(std::string name); ///< \brief Loads the server definition from file.
            ///< \param name Server definition name.
            ///< \return A pointer to loaded server definition.
            ///< \warning At the end of code using this function, call RCF::Client::ServerDefinition::free() function to free the memory.
            static void free(); ///< Frees the memory.
        };
    }
}
#endif
