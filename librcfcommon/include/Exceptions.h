#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H
#include <exception>
#include <string>
#include <boost/filesystem.hpp>
/// \file Exceptions.h
/// \brief Common exceptions for RemoteControlFramework
/// \author Phitherek_
/// \date 2014
/// \version 0.1

/// \namespace RCF
/// \brief A global namespace for RemoteControlFramework
namespace RCF {
    /// \namespace Common
    /// \brief A namespace for common RemoteControlFramework classes
    namespace Common {
        /// \class FilesystemException
        /// \brief An exception related to files and filesystem.
        class FilesystemException: public std::exception {
        private:
            boost::filesystem::path _p;
            std::string _error;
        public:
            FilesystemException(boost::filesystem::path p, std::string error); ///< \brief A constructor from path and error.
            ///< \param p A path related to error.
            ///< \param error What has happened.
            ~FilesystemException() throw(); ///< A destructor required by std::exception.
            const char* what() const throw(); ///< \brief A function describing the error.
            ///< \return What has happened.
        };
        /// \class InvalidObjectException
        /// \brief An exception related to invalidity of object for given action.
        class InvalidObjectException: public std::exception {
        private:
            std::string _error;
        public:
            InvalidObjectException(std::string error); ///< \brief A constructor from error.
            ///< \param error What has happened.
            ~InvalidObjectException() throw(); ///< A destructor required by std::exception.
            const char* what() const throw(); ///< \brief A function describing the error.
            ///< \return What has happened.
        };
        /// \class ParserException
        /// \brief An exception related to parsers and formats.
        class ParserException: public std::exception {
        private:
            boost::filesystem::path _p;
            int _lineno;
            std::string _error;
        public:
            ParserException(boost::filesystem::path p, int lineno, std::string error); ///< \brief A constructor from path, line number and error.
            ///< \param p Path of a file related to error.
            ///< \param lineno Line number where error in the file is.
            ///< \param error What has happened.
            ~ParserException() throw(); ///< A destructor required by std::exception.
            const char* what() const throw(); ///< \brief A function describing the error.
            ///< \return What has happened.
        };
    }
}
#endif
