#ifndef _COMMANDGROUP_H
#define _COMMANDGROUP_H
#include "Command.h"
#include <librcfcommon/librcfcommon.h>
#include <fstream>
/// \file CommandGroup.h
/// \brief A class that represents a group of commands
/// \author Phitherek_
/// \date 2015
/// \version 0.1

/// \namespace RCF
/// \brief A global namespace for RemoteControlFramework
namespace RCF {
    /// \namespace Server
    /// \brief A namespace for RCF server
    namespace Server {
        /// \class CommandGroup
        /// \brief A class that represents a group of commands
        class CommandGroup {
        private:
            std::string _name;
            Command** _commands;
            int _commandsSize;
            int _commandsIterator;
            bool _commandsEnd;
            CommandGroup** _groups;
            int _groupsSize;
            int _groupsIterator;
            bool _groupsEnd;
            static CommandGroup** _loadedGroups;
            static int _loadedGroupsSize;
            void setCommandsEnd();
            void setGroupsEnd();
         public:
             CommandGroup(); ///< A plain constructor.
             CommandGroup(std::string name); ///< \brief A constructor from name.
             ///< \param name Name of a group.
             CommandGroup(CommandGroup& cpy); ///< \brief A copy constructor.
             ///< \param cpy Object to copy.
             ~CommandGroup(); ///< A destructor.
             std::string getName(); ///< \brief Returns the command group' s name.
             ///< \return Command group' s name.
             bool hasCommand(std::string name); ///< \brief Checks if this command group contains command of given name.
             ///< \param name Name of the command.
             ///< \return Boolean.
             bool recursiveHasCommand(std::string name); ///< \brief Checks if this group or any of the groups below contain command of given name.
             ///< \param name Name of the command.
             ///< \return Boolean.
             bool hasGroup(std::string name); ///< \brief Checks if this command group contains command group of given name.
             ///< \param name Name of the command group.
             ///< \return Boolean.
             bool recursiveHasGroup(std::string name); ///< \brief Checks if this group or any of the groups below contain command group of given name.
             ///< \param name Name of the command group.
             ///< \return Boolean.
             void addPermission(Permission* p); ///< \brief Adds permission to all commands in this group.
             ///< \param p A pointer to Permission to add.
             void recursiveAddPermission(Permission* p); ///< \brief Adds permission to all commands in this group and groups below.
             ///< \param p A pointer to Permission to add.
             Command* getNextCommand(); ///< \brief Gets next command in the command group.
             ///< \return Next command in the command group.
             Command* getCommandByName(std::string name); ///< \brief Gets command from command group by name.
             ///< \param name Name of the command.
             ///< \return Command of given name from the command group.
             bool commandsAtEnd(); ///< \brief Checks if commands iterator reached the end.
             ///< \return Boolean.
             void addCommand(Command* c); ///< \brief Adds a command to the command group.
             ///< \param c A command to add.
             void resetCommandsIterator(); ///< Resets the command iterator.
             CommandGroup* getNextGroup(); ///< \brief Gets next command group in the command group.
             ///< \return Next command group in the command group.
             CommandGroup* getGroupByName(std::string name); ///< \brief Gets command group from command group by name.
             ///< \param name Name of the command group.
             ///< \return Command group of given name from the command group.
             bool groupsAtEnd(); ///< \brief Checks if command groups iterator reached the end.
             ///< \return Boolean.
             void addGroup(CommandGroup* cg); ///< \brief Adds a command group to the command group.
             ///< \param cg A command group to add.
             void resetGroupsIterator(); ///< Resets the command groups iterator.
             void save(); ///< Saves the command group.
             static CommandGroup* load(std::string name); ///< Loads command group of given name.
             ///< \param name Name of a command group.
             ///< \return Loaded command group.
             ///< \warning At the end of the code that uses this function, use RCF::Server::CommandGroup::free() function to free the memory, RCF::Server::Command::free() to free the commands if any command was specified inside any group and RCF::Server::Permission::free() to free the permissions if any command had any permission specified.
             bool valid(); ///< \brief Checks if command group is valid.
             ///< \return Boolean.
             static void free(); ///< Frees the memory.
        };
    }
}
#endif
