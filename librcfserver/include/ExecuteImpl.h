#ifndef _EXECUTEIMPL_H
#define _EXECUTEIMPL_H
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <boost/predef.h>
#if BOOST_OS_WINDOWS
#include <windows.h>
#include <tchar.h>
#elif BOOST_OS_LINUX || BOOST_OS_MACOS
#include <unistd.h>
#include <sys/wait.h>
#endif

/// \file ExecuteImpl.h
/// \brief A class that provides cross-platform solution for pipe execution with stdout and stderr capture.
/// \author Phitherek_
/// \date 2014
/// \version 0.1

/// \namespace RCF
/// \brief A global namespace for RemoteControlFramework
namespace RCF {
    /// \namespace Server
    /// \brief A namespace for RCF server
    namespace Server {
        /// \class ExecuteImpl
        /// \brief A class that provides cross-platform solution for pipe execution with stdout and stderr capture.
        class ExecuteImpl {
        private:
            #if BOOST_OS_LINUX || BOOST_OS_MACOS
                static int unix_execute(std::string exec, std::string* stdout_target, std::string* stderr_target);
            #elif BOOST_OS_WINDOWS
                static int windows_execute(std::string exec, std::string* stdout_target, std::string* stderr_target);
            #endif
        public:
            static int execute(std::string exec, std::string* stdout_target, std::string* stderr_target); ///< \brief A cross-platform pipe execution method with stdout and stderr capture.
            ///< \param exec Command to execute.
            ///< \param[out] stdout_target A pointer to target string for stdout.
            ///< \param[out] stderr_target A pointer to target string for stderr.
            ///< \return Command exit value.
        };
    }
}
#endif
