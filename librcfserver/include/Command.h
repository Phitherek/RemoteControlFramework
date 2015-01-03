#ifndef _COMMAND_H
#define _COMMAND_H
#include <string>
#include <vector>
#include "Permission.h"
#include "User.h"
#include "Group.h"
#include "ExecuteImpl.h"
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
            int _permsSize;
            int _permsIterator;
            bool _permsEnd;
            int _numParams;
            void setPermsEnd();
        public:
            Command(); ///< A plain constructor.
            Command(std::string name, std::string exec); ///< \brief A constructor with command name and command on server
            ///< \param name Command name.
            ///< \param exec Command on server.
            Command(std::string name, std::string exec, int numParams); ///< \brief A constructor with command name, command on server and number of parameters.
            ///< \param name Command name.
            ///< \param exec Command on server.
            ///< \param numParams Number of command parameters.
            Command(Command& cpy); ///< \brief A copy constructor.
            ///< \param cpy Object to copy.
            ~Command(); ///< A destructor.
            std::string getName(); ///< \brief A function that returns command name
            ///< \return Command name
            std::string getExec(); ///< \brief A function that returns command to execute on the server
            ///< \return Command on server
            std::string getExec(std::vector<std::string> params); ///< \brief A function that returns command to execute on the server with parameters.
            ///< \param params A std::vector of std::string command parameters.
            ///< \return Command on server with substituted parameters.
            void addPermission(Permission* perm); ///< \brief A function that adds a permission to the command
            ///< \param perm A pointer to a Permission object.
            Permission* getNextPermission(); ///< \brief A function, that returns next permission from the permission array.
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
            bool canExecute(std::string username); ///< \brief If a user of given username can execute this command.
            ///< \param username Name of the user.
            ///< \return If given user can execute the command.
            void save(); ///< Saves command to the server configuration.
            void load(std::string name); ///< \brief Loads the command from the server configuration.
            ///< \param name Name of the command.
            bool valid(); ///< \brief If command is valid.
            ///< \return Boolean.
            int numParams(); ///< \brief Returns the number of parameters that command requires.
            ///< \return Number of parameters.
        };
    }
}
#endif
