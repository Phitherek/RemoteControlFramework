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
    /// \namespace RCF::Common
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
        /// \class NotFoundException
        /// \brief An exception related to not finding object by given string.
        class NotFoundException: public std::exception {
        private:
            std::string _query;
            std::string _error;
        public:
            NotFoundException(std::string query, std::string error); ///< \brief A constructor from query and error.
            ///< \param query Query string that was not found.
            ///< \param error Additional information.
            ~NotFoundException() throw(); ///< A destructor required by std::exception.
            const char* what() const throw(); ///< \brief A function describing the error.
            ///< \return What has happened.
        };
        /// \class AlreadyExistsException
        /// \brief An exception related to object already existing in given collection.
        class AlreadyExistsException: public std::exception {
        private:
            std::string _name;
            std::string _error;
        public:
            AlreadyExistsException(std::string name, std::string error); ///< \brief A constructor from query and error.
            ///< \param name Name of an object that already exists.
            ///< \param error Additional information.
            ~AlreadyExistsException() throw(); ///< A destructor required by std::exception.
            const char* what() const throw(); ///< \brief A function describing the error.
            ///< \return What has happened.
        };
        /// \class AtEndException
        /// \brief An exception related to iterator being at the end of the collection.
        class AtEndException: public std::exception {
        private:
            std::string _error;
        public:
            AtEndException(std::string error); ///< \brief A constructor from error.
            ///< \param error What has happened.
            ~AtEndException() throw(); ///< A destructor required by std::exception.
            const char* what() const throw(); ///< \brief A function describing the error.
            ///< \return What has happened.
        };
        /// \class ParametersNeededException
        /// \brief An exception related to command needing more parameters.
        class ParametersNeededException: public std::exception {
        private:
            int _paramsRequired;
            int _paramsGiven;
            std::string _error;
        public:
            ParametersNeededException(std::string error, int paramsRequired, int paramsGiven); ///< \brief A constructor from params required, params given and error.
            ///< \param paramsRequired A number of parameters that command requires.
            ///< \param paramsGiven A number of parameters that were given to command.
            ///< \param  error Additional error information.
            ~ParametersNeededException() throw(); ///< A destructor required by std::exception.
            const char* what() const throw(); ///< \brief A function describing the error.
            ///< \return What has happened.
        };
        /// \class RuntimeException
        /// \brief An exception related to error during program execution.
        class RuntimeException: public std::exception {
        private:
            std::string _error;
        public:
            RuntimeException(std::string error); ///< \brief A constructor from error.
            ///< \param error What has happened.
            ~RuntimeException() throw(); ///< A destructor required by std::exception.
            const char* what() const throw(); ///< \brief A function describing the error.
            ///< \return What has happened.
        };
        /// \class ProtocolException
        /// \brief An exception related to an error during RCF Protocol communication.
        class ProtocolException: public std::exception {
        private:
            std::string _error;
        public:
            ProtocolException(std::string error); ///< \brief A constructor from error.
            ///< \param error What has happened.
            ~ProtocolException() throw(); ///< A destructor required by std::exception.
            const char* what() const throw(); ///< \brief A function describing the error.
            ///< \return What has happened.
        };
    }
}
#endif
