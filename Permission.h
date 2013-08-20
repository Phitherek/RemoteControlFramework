#ifndef _PERMISSION_H
#define _PERMISSION_H
#include <string>
/// \file Permission.h
/// \brief A class that represents a permission to execute the command
/// \author Phitherek_
/// \date 2013
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
			Permission(std::string name); ///< \brief A constructor with name.
			///< \param name Name of permitted user/group.
			std::string getName(); ///< \brief A function that returns name of permitted user/group.
			///< \return Name of permitted user/group.
			virtual std::string getPermissionType(); ///< \brief A function that returns permission type.
			///< \return Permission type.
		}
	}
}
#endif
