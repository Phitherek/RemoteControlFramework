#ifndef _HELPERFUNCTIONS_H
#define _HELPERFUNCTIONS_H
#include <string>
#include <boost/filesystem.hpp>
/// \file HelperFunctions.h
/// \brief Various helper functions for RemoteControlFramework.
/// \author Phitherek_
/// \date 2014
/// \version 0.1

/// \namespace RCF
/// \brief A global namespace for RemoteControlFramework
namespace RCF {
    /// \namespace Common
    /// \brief A namespace for common RemoteControlFramework classes
    namespace Common {
        /// \class HelperFunctions
        /// \brief A class containing various helper functions for RemoteControlFramework.
        class  HelperFunctions {
        public:
            std::string getPlainHomeDirectory(); ///< \brief A function that gets proper home directory cross-platform - plain version.
            ///< \return Detected home directory as std::string.
            boost::filesystem::path getHomeDirectory(); ///< \brief A function that gets proper home directory cross-platform.
            ///<  \return Detected home directory as boost::filesystem::path.
        };
    } 
}
#endif
