#ifndef _COMMAND_H
#define _COMMAND_H
#include <string>
#include <vector>
#include "Permission.h"
#include "User.h"
#include "Group.h"
/// \file Command.h
/// \brief A class that represents a command
/// \author Phitherek_
/// \date 2014
/// \version 0.1

/// \namespace RCF
/// \brief A global namespace for RemoteControlFramework
namespace RCF {
    /// \namespace Server
    /// \brief A namespace for RCF server
    namespace Server {
        /// \class Command
        /// \brief A class that represents a command for server
        class Command {
        private:
            std::string _name;
            std::string _exec;
            Permission** _perms;
            int _permsIterator;
            bool _permsAtEnd;
            int _numParams;
        public:
            Command(std::string name, std::string exec); ///< \brief A constructor with command name and command on server
            ///< \param name Command name.
            ///< \param exec Command on server.
            Command(std::string name, std::string exec, int numParams); ///< \brief A constructor with command name, command on server and number of parameters.
            ///< \param name Command name.
            ///< \param exec Command on server.
            ///< \param numParams Number of command parameters.
            std::string getName(); ///< \brief A function that returns command name
            ///< \return Command name
            std::string getExec(); ///< \brief A function that returns command to execute on the server
            ///< \return Command on server
            void addPermission(Permission* perm); ///< \brief A function that adds a permission to the command
            ///< \param perm A pointer to a Permission object.
            const Permission* getNextPermission(); ///< \brief A function, that returns next permission from the permission array.
            ///< \return A pointer to the Permission object.
            bool permissionsAtEnd(); ///< \brief Returns if permission iterator is at end.
            ///< \return True if permission iterator is at end, false otherwise.
            void resetPermissionIterator(); ///< A function, that resets permission iterator.
            int execute(std::string* output, std::string* error); ///< \brief A function that executes the command without parameters.
            ///< \param[out] output Command output.
            ///< \param[out] error Command error output.
            ///< \return Command return code.
            int execute(std::string* output, std::string* error, std::vector<std::string> params); ///< \brief A function that executes the command with parameters.
            ///< \param[out] output Command output.
            ///< \param[out] error Command error output.
            ///< \param[in] params A std::vector of std::string command parameters.
            ///< \return Command return code.
        };
    }
}
#endif
