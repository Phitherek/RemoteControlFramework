#ifndef _PERMISSION_H
#define _PERMISSION_H
#include <string>
/// \file Permission.h
/// \brief A class that represents a permission to execute the command
/// \author Phitherek_
/// \date 2014-2015
/// \version 0.1

/// \namespace RCF
/// \brief A global namespace for RemoteControlFramework
namespace RCF {
    /// \namespace RCF::Server
    /// \brief A namespace for RCF server
    namespace Server {
        /// \class Permission
        /// \brief A class that represents a permission to execute the command.
        class Permission {
        protected:
            std::string _name; ///< Name of permitted user/group.
            static Permission** _loadedPermissions; ///< Loaded permissions array.
            static int _loadedPermissionsSize; ///< Size of loaded permissions array.
        public:
            Permission(); ///< A plain constructor.
            Permission(std::string name); ///< \brief A constructor with name.
            ///< \param name Name of permitted user/group.
            virtual ~Permission(); ///< A destructor
            std::string getName(); ///< \brief A function that returns name of permitted user/group.
            ///< \return Name of permitted user/group.
            virtual std::string getPermissionType(); ///< \brief A function that returns permission type.
            ///< \return Permission type.
            virtual void save(); ///< A function that saves permission from file.
            virtual bool valid(); ///< \brief If permission is valid to save or process.
            ///< \return Boolean.
            static void free(); ///< Frees the memory.
        };
    }
}
#endif
