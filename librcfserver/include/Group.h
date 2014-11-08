#ifndef _GROUP_H
#define _GROUP_H
#include "Permission.h"
#include "User.h"
/// \file Group.h
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
        /// \class Group
        /// \brief An implementation of a Group permission.
        class Group: public Permission {
        private:
            User* _users; ///< Users inside group.
            bool valid(); ///< If group is valid to save.
        public:
            Group(); ///< A plain constructor.
            Group(std::string name); ///< \brief A constructor from name.
            ///< \param name Group name.
            void load(std::string name); ///< \brief Load group of given name from configuration.
            ///< \param name Name of group to load.
            void save(); ///< Save group to configuration.
            std::string getPermissionType(); ///< \brief A function that returns permission type.
            ///< \return Permission type.
        };
    }
}
#endif
