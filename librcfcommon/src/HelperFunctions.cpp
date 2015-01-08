#include "HelperFunctions.h"
#include <cstring>
#include <cstdlib>
using namespace RCF::Common;

std::string HelperFunctions::getPlainHomeDirectory() {
    std::string ret;
    char* res;
    res = getenv("HOME");
    if(res == NULL) {
        res = getenv("USERPROFILE");
    }
    ret = "";
    if(res != NULL) {
        for(unsigned int i = 0; i < strlen(res); i++) {
            ret += res[i];
        }
    }
    return ret;
}

boost::filesystem::path HelperFunctions::getHomeDirectory() {
    boost::filesystem::path p(getPlainHomeDirectory());
    return p;
}

void HelperFunctions::setInputEcho(bool e) {
#if BOOST_OS_WINDOWS
    HANDLE stdinHandle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD cmode;
    GetConsoleMode(stdinHandle, &cmode);
    if(e) {
        cmode |= ENABLE_ECHO_INPUT;
    } else {
        cmode &= ~ENABLE_ECHO_INPUT;
    }
    SetConsoleMode(stdinHandle, cmode);
#elif BOOST_OS_LINUX || BOOST_OS_MACOS
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    if(e) {
        term.c_lflag |= ECHO;
    } else {
        term.c_lflag &= ~ECHO;
    }
    (void)tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
}
