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
