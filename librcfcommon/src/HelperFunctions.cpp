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
