#ifndef _USER_H
#define _USER_H
#include "Permission.h"
/// \file User.h
/// \brief An implementation of a user permission.
/// \author Phitherek_
/// \date 2014
/// \version 0.1

/// \namespace RCF
/// \brief A global namespace for RemoteControlFramework
namespace RCF {
    /// \namespace Server
    /// \brief A namespace for RCF server
    namespace Server {
        /// \class User
        /// \brief An implementation of a User permission.
        class User: public Permission {
        private:
            std::string _salt;
            std::string _hashed_password;
            bool valid(); ///< If user is valid to save.
        public:
            User(); ///< A plain constructor.
            User(std::string name, std::string password); ///< \brief A constructor from name and password.
            ///< \param name User name.
            ///< \param password User password (will be encrypted).
            void load(std::string name); ///< \brief Load user of given name from configuration.
            ///< \param name Name of user to load.
            void save(); ///< Save user to configuration.
            std::string getPermissionType(); ///< \brief A function that returns permission type.
            ///< \return Permission type.
        };
    }
}
#endif
