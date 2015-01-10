#ifndef _USER_H
#define _USER_H
#include "Permission.h"
/// \file User.h
/// \brief An implementation of a user permission.
/// \author Phitherek_
/// \date 2014-2015
/// \version 0.1

/// \namespace RCF
/// \brief A global namespace for RemoteControlFramework
namespace RCF {
    /// \namespace RCF::Server
    /// \brief A namespace for RCF server
    namespace Server {
        /// \class User
        /// \brief An implementation of a User permission.
        class User: public Permission {
        private:
            std::string _salt;
            std::string _hashed_password;
        public:
            User(); ///< A plain constructor.
            User(std::string name, std::string password); ///< \brief A constructor from name and password.
            ///< \param name User name.
            ///< \param password User password (will be encrypted).
            ~User(); ///< A destructor
            static User* load(std::string name); ///< \brief Load user of given name from configuration.
            ///< \param name Name of user to load.
            ///< \return A pointer to the instance of loaded User.
            ///< \warning At the end of the code that uses this function, you should call the RCF::Server::Permission::free() function to free the memory.
            void save(); ///< Save user to configuration.
            std::string getPermissionType(); ///< \brief A function that returns permission type.
            ///< \return Permission type.
            bool checkPassword(std::string password);  ///< \brief A function that returns if given password for user is correct.
            ///< \param password Password to check.
            ///< \return True if password matches, false otherwise.
            bool valid(); ///< \brief If permission is valid to save or process.
            ///< \return Boolean.
            static void free(); ///< Free the memory.
        };
    }
}
#endif
