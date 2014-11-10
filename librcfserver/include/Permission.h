#ifndef _PERMISSION_H
#define _PERMISSION_H
#include <string>
/// \file Permission.h
/// \brief A class that represents a permission to execute the command
/// \author Phitherek_
/// \date 2014
/// \version 0.1

/// \namespace RCF
/// \brief A global namespace for RemoteControlFramework
namespace RCF {
    /// \namespace Server
    /// \brief A namespace for RCF server
    namespace Server {
        /// \class Permission
        /// \brief A class that represents a permission to execute the command.
        class Permission {
        protected:
            std::string _name; ///< Name of permitted user/group.
        public:
            Permission(); ///< A plain constructor.
            Permission(std::string name); ///< \brief A constructor with name.
            ///< \param name Name of permitted user/group.
            std::string getName(); ///< \brief A function that returns name of permitted user/group.
            ///< \return Name of permitted user/group.
            virtual std::string getPermissionType(); ///< \brief A function that returns permission type.
            ///< \return Permission type.
            virtual void load(std::string name); ///< \brief A function that loads permission from file.
            ///< \param name Permission name.
            virtual void save(); ///< A function that saves permission from file.
        };
    }
}
#endif
