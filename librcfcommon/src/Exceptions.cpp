#include "Exceptions.h"
#include <sstream>
using namespace RCF::Common;

FilesystemException::FilesystemException(boost::filesystem::path p, std::string error) {
    _p = p;
    _error = error;
}

FilesystemException::~FilesystemException() throw() {}

const char* FilesystemException::what() const throw() {
    std::string msg = "";
    msg += _p.string();
    msg += ": ";
    msg += _error;
    return msg.c_str();
}

InvalidObjectException::InvalidObjectException(std::string error) {
    _error = error;
}

InvalidObjectException::~InvalidObjectException() throw() {}

const char* InvalidObjectException::what() const throw() {
    return _error.c_str();
}

ParserException::ParserException(boost::filesystem::path p, int lineno, std::string error) {
    _p = p;
    _lineno = lineno;
    _error = error;
}

ParserException::~ParserException() throw() {}

const char* ParserException::what() const throw() {
    std::string ret = "";
    ret += _p.string();
    ret += ":";
    std::stringstream nrss;
    nrss.str("");
    nrss << _lineno;
    ret += nrss.str();
    ret += ": ";
    ret += _error;
    return ret.c_str();
}

NotFoundException::NotFoundException(std::string query, std::string error) {
    _query = query;
    _error = error;
}

NotFoundException::~NotFoundException() throw() {}

const char* NotFoundException::what() const throw() {
    std::string ret = "";
    ret += _query;
    ret += ": ";
    ret += _error;
    return ret.c_str();
}

AlreadyExistsException::AlreadyExistsException(std::string name, std::string error) {
    _name = name;
    _error = error;
}

AlreadyExistsException::~AlreadyExistsException() throw() {}

const char* AlreadyExistsException::what() const throw() {
    std::string ret = "";
    ret += _name;
    ret += ": ";
    ret += _error;
    return ret.c_str();
}

AtEndException::AtEndException(std::string error) {
    _error = error;
}

AtEndException::~AtEndException() throw() {}

const char* AtEndException::what() const throw() {
    return _error.c_str();
}

ParametersNeededException::ParametersNeededException(std::string error, int paramsRequired, int paramsGiven) {
    _paramsRequired = paramsRequired;
    _paramsGiven = paramsGiven;
    _error = error;
}

ParametersNeededException::~ParametersNeededException() throw() {}

const char* ParametersNeededException::what() const throw() {
    std::stringstream retss;
    retss.str("");
    retss << _error << " (" << _paramsRequired << " parameters required, " << _paramsGiven << " given)";
    return retss.str().c_str();
}

RuntimeException::RuntimeException(std::string error) {
    _error = error;
}

RuntimeException::~RuntimeException() throw() {}

const char* RuntimeException::what() const throw() {
    return _error.c_str();
}

