#ifndef _GROUP_H
#define _GROUP_H
#include "Permission.h"
#include "User.h"
#include <string>
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
            User** _users; ///< Users inside group.
            int _usersSize; ///< Size of users.
            int _usersIterator; ///< Iterator for users.
            bool _usersEnd; ///< If users table is at end.
            void setUsersEnd();
        public:
            Group(); ///< A plain constructor.
            ~Group(); ///< A destructor.
            Group(std::string name); ///< \brief A constructor from name.
            ///< \param name Group name.
            Group(Group& cpy); ///< \brief A copy constructor.
            ///< \param cpy Object to copy.
            static Group* load(std::string name); ///< \brief Load group of given name from configuration.
            ///< \param name Name of group to load.
            ///< \return Found Group.
            void save(); ///< Save group to configuration.
            std::string getPermissionType(); ///< \brief A function that returns permission type.
            ///< \return Permission type.
            void addUser(User* u);  ///< \brief Adds a user to group.
            ///< \param u A pointer to User to add.
            void removeUser(std::string name); ///< \brief Removes user from group.
            ///< \param name Name of the user.
            bool hasUser(std::string name); ///< \brief Checks if user of given name belongs to group.
            ///< \param name Name of the user.
            ///< \return If user belongs to group.
            User* getNextUser(); ///< \brief Gets next user from the group based on internal iterator.
            ///< \return Next user from the group.
            bool usersAtEnd(); ///< \brief Checks if users iterator is at the end.
            ///< \return If users iterator is at the end.
            void resetUsersIterator(); ///< Resets users internal iterator.
            bool valid(); ///< \brief If permission is valid to save or process.
            ///< \return Boolean.
        };
    }
}
#endif
