#ifndef _COMMAND_H
#define _COMMAND_H
#include <string>
#include "Permission.h"
#include "User.h"
#include "Group.h"
/// \file Command.h
/// \brief A class that represents a command
/// \author Phitherek_
/// \date 2013
/// \version 0.1

/// \namespace RCF
/// \brief A global namespace for RemoteControlFramework
namespace RCF {
	/// \namespace Client
	/// \brief A namespace for RCF clients
	namespace Client {
		/// \class Command
		/// \brief A class that represents a command for client
		class Command {
		protected:
			std::string _name; ///< Command name
		public:
			Command(std::string name); ///< \brief A constructor with command name
			///< \param name Command name
			std::string getName(); ///< \brief A function that returns command name
			///< \return Command name
		};
	}
	/// \namespace Server
	/// \brief A namespace for RCF server
	namespace Server {
		/// \class Command
		/// \brief A class that represents a command for server
		class Command: public Client::Command {
		private:
			std::string _exec;
			Permission** _perms;
			int _permsIterator;
			bool _permsAtEnd;
		public:
			Command(std::string name, std::string exec); ///< \brief A constructor with command name and command on server
			///< \param name Command name
			///< \param exec Command on server
			std::string getExec(); ///< \brief A function that returns command to execute on the server
			///< \return Command on server
			void addPermission(Permission* perm); ///< \brief A function that adds a permission to the command
			///< \param perm A pointer to a Permission object.
			const Permission* getNextPermission(); ///< \brief A function, that returns next permission from the permission array.
			///< \return A pointer to the Permission object.
			bool permissionsAtEnd(); ///< \brief Returns if permission iterator is at end.
			///< \return True if permission iterator is at end, false otherwise.
			void resetPermissionIterator(); ///< A function, that resetes permission iterator.
		};
	}
}
#endif
