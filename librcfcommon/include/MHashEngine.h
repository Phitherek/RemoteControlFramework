#ifndef _MHASHENGINE_H
#define _MHASHENGINE_H
#include "mhash.h"
#include <string>
/// \file MHashEngine.h
/// \brief A class that handles MD5 encryption using MHash library (compile with -lmhash)
/// \author Phitherek_
/// \date 2014
/// \version 0.1

/// \namespace RCF
/// \brief A global namespace for RemoteControlFramework
namespace RCF {
    /// \namespace RCF::Common
    /// \brief A namespace for common RemoteControlFramework classes
    namespace Common {
        /// \class MHashEngine
        /// \brief A class that handles MD5 encryption using MHash
        class MHashEngine {
        private:
            unsigned char* _out;
            int _outsize;
            MHASH _thread;
        public:
            MHashEngine(); ///< A constructor initializing MHash
            ~MHashEngine(); ///< A destructor deinitializing MHash
            std::string getHash(std::string in); ///< \brief A function that calculates MD5 hash of given string and returns it in hexadecimal form
            ///< \param in A string to encrypt
            ///< \return A MD5 hash of the string in hexadecimal form
        };
    }
}
#endif
